#include "TaskCmp.h"

#include <queue>
#include <algorithm>

#include <pthread.h>

using namespace std;

class TaskBase;

class Queue
{
	public:
		static void start();
		static void wait();
		static void push(TaskBase * task);
		static void pop();

	private:
		static priority_queue<TaskBase*, vector<TaskBase*>, TaskCmp> * tasks;

		static pthread_t pth;
		static pthread_mutex_t mtx;
		static pthread_cond_t cnd;

		static void * process(void *);
};
