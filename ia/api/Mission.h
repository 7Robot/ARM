#ifndef _MISSION_H_
#define _MISSION_H_

#include <pthread.h>

#include "Can.h"
#include "MissionHandler.h"

class MissionHandler;

class Mission
{
	public:
		void setup(Can * can);
		//void init();
		virtual void run() = 0;
		int getState() { return state; }
		char * getName() { return name; }
		virtual void mission() {}
		virtual bool microswitch(int id, bool status) { return true; }
		virtual bool asserv(int error) { return true; }
		virtual bool sonar(int id, bool edge, bool nearby, int distance) { return true; }
		virtual bool odometry(int x, int y, int theta) { return true; }

	protected:
		Can * can;
		int state;
		char * name;
		//void wait();
		//void signal();
		void load(char * mission);
		void end();

	private:
		//pthread_cond_t cnd;
		//pthread_mutex_t mtx;
		//char * next;
		int runthreadid;
		int signaux;
};

#endif
