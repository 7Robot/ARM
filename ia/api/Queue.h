#include <queue>
#include <algorithm>

#include <pthread.h>
#include "Task.h"
#include "TaskCmp.h"

using namespace std;

class Task;

class Queue
{
	public:
		static void start();
		static void wait();
		static void push(Task * task);
		static void pop();

		static priority_queue<Task*, vector<Task*>, TaskCmp> * tasks;
	private:
		static pthread_t pth;
		static pthread_mutex_t mtx;
		static pthread_cond_t cnd;

		static void * process(void *);
};
