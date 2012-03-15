#include "TaskCmp.h"

#include <queue>

#include <pthread.h>

using namespace std;

class Task;

class Queue
{
	public:
		static void start();
		static void wait(); // deprecated
		static void push(Task * task);
		static void pop();

	private:
		static priority_queue<Task*, vector<Task*>, TaskCmp> * tasks;

		static pthread_t pth;
		static pthread_mutex_t mtx;
		static pthread_cond_t cnd;

		static void * process(void *);
};
