#include "cv.h"
#include "mutex.h"
#include "cpu.h"

using namespace std;

class cv::impl{
public:
	queue<ucontext_t *> waiting_queue;
};

cv::cv(){
	impl_ptr = new impl();
}
cv::~cv(){
	delete impl_ptr;
}
void cv::wait(mutex&){
	//a thread always calls wait() while holding a lock, and the call to wait atomically releases the lock
	//and puts the threads on the condition variable's waiting queue.
	cpu::interrupt_disable();
	mutex->unlock();
	impl_ptr->waiting_queue.push(getcontext(ucontext_ptr));

	cpu::interrupt_enable_suspend();
	mutex->lock();
}

void cv::signal(){
	cpu::interrupt_disable();
	if(impl_ptr->waiting_queue.size() > 0){
		ready_queue.push(impl_ptr->waiting_queue.pop());
	}
	cpu::interrupt_send();
	cpu::interrupt_enable();
}

void cv::broadcast(){
	cpu::interrupt_disable();
	while(impl_ptr->waiting_queue.size() > 0){
		ready_queue.push(impl_ptr->waiting_queue.pop());
	}
	cpu::interrupt_send();
	cpu::interrupt_enable();
}