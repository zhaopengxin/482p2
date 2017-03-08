#include "cv_impl.h"
cv::cv(){
	try {
		impl_ptr = new impl();
	} catch(bad_alloc &e) {
		throw e;
	}
}
cv::~cv(){
	delete impl_ptr;
}
void cv::wait(mutex& m){
	// A thread always calls wait() while holding a lock, 
	// and the call to wait atomically releases the lock
	// And puts the threads on the condition variable's waiting queue.
	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));

	assert(cpu::self()->impl_ptr->cur_context);
	if (m.impl_ptr->free || 
		m.impl_ptr->lock_holder != cpu::self()->impl_ptr->cur_context) {
		guard.store(false);
		cpu::interrupt_enable();
		throw std::runtime_error("wait without hold the lock");
	}

	impl_ptr->waiting_queue.push(cpu::self()->impl_ptr->cur_context); // wait in waiting queue
 

	m.impl_ptr->unlock_helper();     // release lock
	cpu::impl::switch_context(cpu::self()->impl_ptr->cur_context);  
	m.impl_ptr->lock_helper();		 // get lock again

	assert(guard.load());
	assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();
}

void cv::signal(){
	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));

	if(impl_ptr->waiting_queue.size() > 0){
		ucontext_t *to_ready = impl_ptr->waiting_queue.front();
		cpu::impl::ready_queue.push(to_ready);
		cpu::impl::cpu_wakeup();
		impl_ptr->waiting_queue.pop();	
	}
	//assert(guard.load());
	//assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();
}

void cv::broadcast(){
	assert_interrupts_enabled();

	cpu::interrupt_disable();
	while (guard.exchange(true));

	while(impl_ptr->waiting_queue.size() > 0){
		ucontext_t *to_ready = impl_ptr->waiting_queue.front();
		cpu::impl::ready_queue.push(to_ready);                 
		cpu::impl::cpu_wakeup();
		impl_ptr->waiting_queue.pop();	
	}

	//assert(guard.load());
	//assert_interrupts_disabled();

	guard.store(false);
	cpu::interrupt_enable();
}
