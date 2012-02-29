#ifndef _MISSION_H_
#define _MISSION_H_

#include <pthread.h>

#include "Can.h"
#include "MissionHandler.h"

class MissionHandler;

class Mission
{
	public:
		void setup(MissionHandler * mh, Can * can);
		void init();
		virtual bool microswitch(int id, bool status) {}
		virtual bool asserv(int error) {}
		virtual bool sonar(int id, bool edge, bool nearby, int distance) {}
		virtual bool odometry(int x, int y, int theta) {}

	protected:
		Can * can;
		int state;
		void wait();
		void signal();
		void load(char * mission);

	private:
		virtual void run() = 0;
		MissionHandler * mh;
		pthread_cond_t cnd;
		pthread_mutex_t mtx;
		char * next;
};

#endif
