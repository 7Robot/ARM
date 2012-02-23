#ifndef _MISSION_H_
#define _MISSION_H_

#include "Can.h"

class MissionHandler;

class Mission
{
	public:
		void setup(MissionHandler * mh, Can * can) {
			this->mh = mh;
			this->can = can;
		}

		virtual void init() = 0;
		virtual void run() = 0;
	
		virtual void microswitch(int id, bool status) {}
		virtual void asserv(int error) {}
		virtual void sonar(int id, bool edge, bool nearby, int distance) {}
		virtual void odometry(int x, int y, int theta) {}

	protected:
		MissionHandler * mh;
		Can * can;
		int state;

};

#endif
