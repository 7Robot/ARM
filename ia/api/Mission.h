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

		virtual void init() = 0;
		virtual void run() = 0;
	
		virtual void microswitch(int id, bool status) {}
		virtual void asserv(int error) {}
		virtual void sonar(int id, bool edge, bool nearby, int distance) {}
		virtual void odometry(int x, int y, int theta) {}

		//char * getnext();

	protected:
		MissionHandler * mh;
		Can * can;
		int state;
		//void setnext(char*);
		void wait();
		void signal();

	private:
		pthread_cond_t cnd;
		pthread_mutex_t mtx;
		//char ** next;
};

#endif
