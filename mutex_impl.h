#ifndef _MUTEX_IMPL_H
#define _MUTEX_IMPL_H

# include "mutex.h"
# include "cpu_impl.h"      // sunby 2.9

class mutex::impl {
public:
	void lock_helper();

	void unlock_helper();

	queue<ucontext_t *> waiting_queue;

	ucontext_t* lock_holder;  // siyao 2.12

	bool free = true;                      
};

#endif
