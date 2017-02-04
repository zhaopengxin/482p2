#include "cv.h"

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
	
}

void cv::signal(){

}

void cv::broadcast(){

}