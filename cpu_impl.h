#ifndef _CPU_IMPL_H
#define _CPU_IMPL_H

#include <ucontext.h>
#include <atomic>
#include <queue>
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "cpu.h"
using namespace std;

class cpu::impl {
public: 
	impl(){}
	~impl(){}
	/*
	 * A helper function for the context switching.
	 * Switch from the current context to the context at the front of the ready_queue
	 * If ready_queue is empty, return to the main context (init) of the cpu.
	 */
	static void switch_context(ucontext_t* cur);

	/*
	 * A helper function to delete threads in finished_queue.
	 */
	static void delete_finished();

	/*
	 * A helper function to signal a suspending cpu in the suspended_queue
	 */
	static void cpu_wakeup();


	static queue<ucontext_t*> ready_queue;
	static queue<ucontext_t*> finished_queue;
	static queue<cpu*>        suspended_queue;

	ucontext_t *cur_context;   // store the current context
	ucontext_t *main_context;  // the main context that cpu is running in (init)
};

#endif 

