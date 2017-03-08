#ifndef _THREAD_IMPL_H
#define _THREAD_IMPL_H

#include "thread.h"
#include "cpu_impl.h"

//using namespace std;

extern queue<ucontext_t*> cpu::impl::ready_queue;
extern queue<ucontext_t*> cpu::impl::finished_queue;
extern queue<cpu*> cpu::impl::suspended_queue;

class thread::impl {
public:
	/*
	 * Allocate the new thread, and 
	 * put the new thread into ready queue
	 */
	impl(thread_startfunc_t func, void *arg); 

	/* The package function in which user function is conducted
	 * 1. Inorder to maintain the invariants that interrupts must be 
	 * enabled in user function.
	 * 2. Inorder to delete the thread (if appropriate) after user function 
	 */
	static void function(thread_startfunc_t func, void *arg, thread::impl* impl_ptr);      

	ucontext_t *tcb;                   // Context info of the thread
	queue<ucontext_t*> waiting_queue;  // waiting queue for thread::join()

	/*
	 * To delete the thread after:
	 *     the thread destructed &&
	 *     the thread complete its function
	 */
	bool obj_finished = 0;   // Set if the thread get destructed
	bool func_finished = 0;  // Set if the thread complete its function
};

#endif
