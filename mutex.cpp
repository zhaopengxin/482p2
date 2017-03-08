#include "mutex_impl.h"
void mutex::impl::lock_helper() {
	if(free == true) {
		assert(cpu::self()->impl_ptr->cur_context); //siyao 2.12
		lock_holder = cpu::self()->impl_ptr->cur_context;// siyao 2.12
		free = false;
	} else {
		assert(cpu::self()->impl_ptr->cur_context);       		
		waiting_queue.push(cpu::self()->impl_ptr->cur_context);
		cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);          		
	}
}

void mutex::impl::unlock_helper() {
	free = true;
	if (waiting_queue.size() > 0) {
		ucontext_t *next = waiting_queue.front();
		waiting_queue.pop();
		cpu::impl::ready_queue.push(next);
		lock_holder = cpu::impl::ready_queue.back(); 
		cpu::impl::cpu_wakeup();
		free = false;
	} // hand-off lock
}

mutex::mutex() {
	try {
		impl_ptr = new impl();
	} catch(exception &e) {
		throw e;
	}
}

mutex::~mutex() {
	delete impl_ptr;
}

void mutex::lock() {                        
	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));
		
	impl_ptr->lock_helper();

	assert(guard.load());
	assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();
}

void mutex::unlock() {                    
	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));

	assert(cpu::self()->impl_ptr->cur_context);
	if (impl_ptr->free || 
		impl_ptr->lock_holder != cpu::self()->impl_ptr->cur_context) {
		guard.store(false);
		cpu::interrupt_enable();
		throw std::runtime_error("wait without holding the lock");
	}

	impl_ptr->unlock_helper();

	//assert(guard.load());
	//assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();                
}

