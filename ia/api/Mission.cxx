#include "Mission.h"

void Mission::setup(MissionHandler * mh, Can * can)
{
		this->mh = mh;
		this->can = can;
		cnd = PTHREAD_COND_INITIALIZER;
		mtx = PTHREAD_MUTEX_INITIALIZER;
}

void Mission::wait()
{
	printf("Mission::wait()\n");

	pthread_mutex_lock(&mtx);
	pthread_cond_wait(&cnd, &mtx);
}

void Mission::signal()
{
	printf("Mission::signal()\n");

	pthread_mutex_lock(&mtx);
	pthread_cond_signal(&cnd);
	pthread_mutex_unlock(&mtx);
}
