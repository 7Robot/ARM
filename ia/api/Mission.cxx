#include "Mission.h"

void Mission::setup(MissionHandler * mh, Can * can)
{
	this->mh = mh;
	this->can = can;
	cnd = PTHREAD_COND_INITIALIZER;
	mtx = PTHREAD_MUTEX_INITIALIZER;
	next = NULL;
	runthreadid = pthread_self();
}

void Mission::init()
{
	printf("Minit 1\n");
	run();
	printf("Minit 2\n");
	if (next == NULL) {
		printf("Minit 2.4\n");
		wait();
		printf("Minit 2.6\n");
	}
	printf("Minit 3\n");
	while (next != NULL) {
		printf("Minit 3.3\n");
		mh->load(next);
		printf("Minit 3.4\n");
		next = NULL;
		printf("Minit 3.5\n");
		wait();
		printf("Minit 3.6\n");
	}
	printf("Minit 4\n");
}

void Mission::wait()
{
	printf("Mission::wait()\n");

	pthread_mutex_lock(&mtx);
	pthread_cond_wait(&cnd, &mtx);

	printf("Mission::unwait()\n");
}

void Mission::signal()
{
	printf("Mission::signal()\n");

	pthread_mutex_lock(&mtx);
	pthread_cond_signal(&cnd);
	pthread_mutex_unlock(&mtx);
}

void Mission::load(char * mission)
{
	printf("Mission::load(%s)\n", mission);
	next = mission;
	signal();
}
