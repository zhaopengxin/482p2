#include "thread_impl.h"

thread::impl::impl(thread_startfunc_t func, void *arg) {
	assert_interrupts_enabled();
	cpu::interrupt_disable();		
	while (guard.exchange(true));

	cpu::impl::delete_finished();
	try {
		tcb = new ucontext_t();
		getcontext(tcb);

		char *stack = new char[STACK_SIZE];
		tcb->uc_stack.ss_sp = stack;
		tcb->uc_stack.ss_size = STACK_SIZE;
		tcb->uc_stack.ss_flags = 0;
		tcb->uc_link = nullptr;
	} catch(bad_alloc &e) {
		guard.store(false);
		cpu::interrupt_disable();
		throw e;
	}

	makecontext(tcb, (void (*)())function, 3, func, arg, this);
	cpu::impl::ready_queue.push(tcb);

	cpu::impl::cpu_wakeup();

	assert(guard.load());
	assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();
}

void thread::impl::function(thread_startfunc_t func, void *arg, thread::impl* impl_ptr) {
	assert(guard.load());
	assert_interrupts_disabled();

	cpu::impl::delete_finished();  // Avoid the case that many threads in the ready_q and cannot wait for the delete

	guard.store(false);
	cpu::interrupt_enable();

	func(arg);

	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));

	cpu::impl::delete_finished();  // Avoid the case that many threads in the ready_q and cannot wait for the delete
	assert(cpu::self()->impl_ptr->cur_context);
	cpu::impl::finished_queue.push(cpu::self()->impl_ptr->cur_context);
	
	while (!impl_ptr->waiting_queue.empty()) {
		ucontext_t *wait_c = impl_ptr->waiting_queue.front();
		impl_ptr->waiting_queue.pop();
		cpu::impl::ready_queue.push(wait_c);
		cpu::impl::cpu_wakeup();
	}

	// delete 
	assert(impl_ptr);
	impl_ptr->func_finished = 1;
	if (impl_ptr->obj_finished) {
		delete impl_ptr;
		impl_ptr = nullptr;
	}

	cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);

	assert(guard.load());
	assert_interrupts_disabled();
	guard.store(false);
	cpu::interrupt_enable();	
}



thread::thread(thread_startfunc_t func, void *arg) {
	try {
		impl_ptr = new impl(func, arg);
	} catch(bad_alloc &e) {
		throw e;
	}
}

thread::~thread() {
	assert_interrupts_enabled();

	cpu::interrupt_disable();		
	while (guard.exchange(true));

	assert(impl_ptr);
	impl_ptr->obj_finished = 1;
	if (impl_ptr->func_finished) {
		delete impl_ptr;
		impl_ptr = nullptr;
	}
	
	guard.store(false);
	cpu::interrupt_enable();		
}

void thread::join() {
	assert_interrupts_enabled();

	cpu::interrupt_disable();		
	while (guard.exchange(true));

	assert(cpu::self()->impl_ptr->cur_context);
	if (impl_ptr->func_finished) {
		cpu::impl::ready_queue.push(cpu::self()->impl_ptr->cur_context);
		cpu::impl::cpu_wakeup();
	} else {
		impl_ptr->waiting_queue.push(cpu::self()->impl_ptr->cur_context);
	}
	
	cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);

	assert(guard.load());
	assert_interrupts_disabled();
	guard.store(false);
	cpu::interrupt_enable();
}

void thread::yield() {
	assert_interrupts_enabled();
	cpu::interrupt_disable();
	while (guard.exchange(true));
	
	if (!cpu::impl::ready_queue.empty()) {
		assert(cpu::self()->impl_ptr->cur_context);
		cpu::impl::ready_queue.push(cpu::self()->impl_ptr->cur_context);
		cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);
	}
	
	assert_interrupts_disabled();
	guard.store(false);
	cpu::interrupt_enable();		
}
