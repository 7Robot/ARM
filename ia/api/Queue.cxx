#include "Queue.h"

#include <stdio.h>

using namespace std;

pthread_t Queue::pth;
pthread_mutex_t Queue::mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Queue::cnd = PTHREAD_COND_INITIALIZER;
priority_queue<Task*, vector<Task*>, TaskCmp> * Queue::tasks = new priority_queue<Task*, vector<Task*>, TaskCmp>();

void Queue::start()
{
	pthread_create(&pth, NULL, Queue::process, NULL);
}

void Queue::wait()
{
	pthread_join(pth, NULL);
}

void * Queue::process(void *)
{
	while (1) {
		pthread_mutex_lock(&mtx);
		while (!tasks->empty()) {
			Task * t = tasks->top();
			tasks->pop();
			pthread_mutex_unlock(&mtx);
			(*t)();
			pthread_mutex_lock(&mtx);
		}
		pthread_cond_wait(&cnd, &mtx);
		pthread_mutex_unlock(&mtx);
	}
}

void Queue::push(Task * task)
{
	pthread_mutex_lock(&Queue::mtx);
	tasks->push(task);
	pthread_cond_signal(&cnd);
	pthread_mutex_unlock(&Queue::mtx);
}
