#include "Mission.h"

void Mission::setup(Can * can)
{
	this->can = can;
	//cnd = PTHREAD_COND_INITIALIZER;
	//mtx = PTHREAD_MUTEX_INITIALIZER;
	//next = NULL;
	runthreadid = pthread_self();
	name = "<mission name>";
	signaux = 0;
}

void Mission::end()
{
	printf("Mission::end\n");
	//MissionHandler::unload();
	MissionHandler::next = "unload";
	MissionHandler::signal();
}

void Mission::load(char * mission)
{
	/*if (runthreadid == (int)pthread_self() && MissionHandler::unload) {
		printf("Mission::load(%s): run thread\n", mission);
		MissionHandler::load(mission);
	} else {*/
		printf("Mission::load(%s): send signal to run thread\n", mission);
		MissionHandler::next = mission;
		MissionHandler::signal();
	//}
}

/*void Mission::wait()
{
	printf("Mission::wait()\n");

	pthread_mutex_lock(&mtx);
	if (signaux > 0) {
		signaux--;
		pthread_mutex_unlock(&mtx);
	} else {
		pthread_cond_wait(&cnd, &mtx);
		pthread_mutex_lock(&mtx);
		signaux--;
		pthread_mutex_unlock(&mtx);
	}

	printf("Mission::unwait()\n");
}

void Mission::signal()
{
	printf("Mission::signal()\n");

	pthread_mutex_lock(&mtx);
	signaux++;
	pthread_cond_signal(&cnd);
	pthread_mutex_unlock(&mtx);
}*/
