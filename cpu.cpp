#include "cpu_impl.h"

void cpu::impl::switch_context(ucontext_t* cur) {
	// check invariance before context switch
	assert_interrupts_disabled();
	assert(guard.load());

	ucontext_t *next;
	if (cpu::impl::ready_queue.empty()) {
		cpu::self()->impl_ptr->cur_context = cpu::self()->impl_ptr->main_context;    // before switch, change cur_context to the next context
		swapcontext(cur, cpu::self()->impl_ptr->main_context);
	} else {
		next = cpu::impl::ready_queue.front();
		cpu::impl::ready_queue.pop();
		cpu::self()->impl_ptr->cur_context = next;      // before switch, change cur_context to the next context
		swapcontext(cur, next);	
	}
	cpu::impl::delete_finished();
}

void cpu::impl::delete_finished() {
	assert_interrupts_disabled();
	assert(guard.load());
	
	while (!cpu::impl::finished_queue.empty()) {
		ucontext_t *to_del = cpu::impl::finished_queue.front();
		cpu::impl::finished_queue.pop();
		delete[] (char*)to_del->uc_stack.ss_sp;
		delete to_del;
	}
}

void cpu::impl::cpu_wakeup() {
	assert_interrupts_disabled();
	assert(guard.load());

	if (!cpu::impl::suspended_queue.empty())	{
		cpu* to_wake = suspended_queue.front();
		suspended_queue.pop();
		to_wake->interrupt_send();
	}
}

static void timer_handler() {
	if (cpu::self()->impl_ptr->cur_context != cpu::self()->impl_ptr->main_context) 
	{
		thread::yield();
	}
}

static void IPI_handler() {
	assert_interrupts_enabled();
	cpu::interrupt_disable();
}

/*
 * 1. Allocate impl_ptr
 * 2. Setup interrupt_vector_table
 * 3. Start a new thread
 * 4. Loop, suspend if no runnable threads.
 */
void cpu::init(thread_startfunc_t func, void *arg) {
		
	// initialize impl_ptr
	try {
		impl_ptr = new impl();
	} catch(bad_alloc &e) {
		throw e;
	}

	// setup interrupt_vector_table
	interrupt_vector_table[TIMER] = (interrupt_handler_t)timer_handler;		
	interrupt_vector_table[IPI] = (interrupt_handler_t)IPI_handler;		

	// a new thread for the func
	cpu::interrupt_enable();
	if (func) {
		thread t(func, arg);
	}
	cpu::interrupt_disable();

	// main_context and cur_context
	try {
		impl_ptr->main_context = new ucontext_t();
	} catch(bad_alloc &e) {
		throw e;
	}

	getcontext(impl_ptr->main_context);
	cpu::self()->impl_ptr->cur_context = impl_ptr->main_context;

	while (true) {
		assert_interrupts_disabled();
		while (guard.exchange(true));
		while (!cpu::impl::ready_queue.empty()) {
			//cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);	
			cpu::impl::switch_context(impl_ptr->cur_context);	
		}			

		// Suspend
		cpu::impl::delete_finished();
		cpu::impl::suspended_queue.push(cpu::self());

		assert(guard.load());
		assert_interrupts_disabled();

		guard.store(false);
		interrupt_enable_suspend();
	}
}

