#include "cv.h"
#include "mutex_impl.h"
#include "cpu_impl.h"  

//using namespace std;

class cv::impl{
public:
	queue<ucontext_t *> waiting_queue;
};


