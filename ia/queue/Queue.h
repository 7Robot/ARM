#include "TaskCmp.h"

#include <queue>
#include <thread>

#include <pthread.h>

using namespace std;

class Task;

class Queue
{
	public:
		Queue();
		//~Queue();
		void push(Task * task);
		void pop();

	private:
		priority_queue<Task*, vector<Task*>, TaskCmp> tasks;

		//pthread_t pth;
		std::thread thr;
		pthread_mutex_t mtx;
		pthread_cond_t cnd;

		void process();
};
