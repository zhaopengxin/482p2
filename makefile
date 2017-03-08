test_thread_join: test_thread_join.cpp thread.cpp cpu.cpp cv.cpp mutex.cpp
	g++ test_thread_join.cpp thread.cpp cpu.cpp cv.cpp mutex.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_disk_scheduler: test_disk_scheduler.cpp thread.cpp cv.cpp cpu.cpp mutex.cpp
	g++ test_disk_scheduler.cpp thread.cpp cv.cpp cpu.cpp mutex.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_thread_yield: test_thread_yield.cpp thread.cpp cv.cpp cpu.cpp mutex.cpp
	g++ test_thread_yield.cpp thread.cpp cv.cpp cpu.cpp mutex.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_thread_cons_des: test_thread_cons_des.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_thread_cons_des.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test4: test4.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test4.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv_signal: test_cv_signal.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv_signal.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv_broadcast: test_cv_broadcast.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv_broadcast.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv: test_cv.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_mutex: test_mutex.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_mutex.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv_waitingqueue: test_cv_waitingqueue.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv_waitingqueue.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv_waitingqueue2: test_cv_waitingqueue2.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv_waitingqueue2.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_cv_wait_without_lock: test_cv_wait_without_lock.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_cv_wait_without_lock.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
test_fifo: test_fifo.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp
	g++ test_fifo.cpp thread.cpp cv.cpp mutex.cpp cpu.cpp libcpu.o -o main -ldl -pthread -std=c++11 -g
