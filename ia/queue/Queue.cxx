#include "Queue.h"
#include "Task.h"

#include <stdio.h>

using namespace std;

Queue::Queue(): thr(&Queue::process, this) {}

void Queue::process()
{
	mtx = PTHREAD_MUTEX_INITIALIZER;
	cnd = PTHREAD_COND_INITIALIZER;
	while (1) {
		pthread_mutex_lock(&mtx);
		while (!tasks.empty()) {
			Task * t = tasks.top();
			tasks.pop();
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
	pthread_mutex_lock(&mtx);
	tasks.push(task);
	pthread_cond_signal(&cnd);
	pthread_mutex_unlock(&mtx);
}
