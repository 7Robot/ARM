#ifndef _MISSION_H_
#define _MISSION_H_

#include <pthread.h>

#include "Can.h"
#include "MissionHandler.h"

class MissionHandler;

class Mission
{
	public:
		void setup(Can * can, const char * name);

		int getState() const;
		const char * getName() const;

		virtual void start() {};

		virtual void mission() {}
		virtual bool microswitch(int id, bool status) { return true; }
		virtual bool asserv(int error) { return true; }
		virtual bool sonar(int id, bool edge, bool nearby, int distance) { return true; }
		virtual bool odometry(int x, int y, int theta) { return true; }

		virtual void stop() {};

	protected:
		Can * can;
		int state;
		void load(const char * mission);
		void end();

	private:
		int m_id;
		char * name;
};

#endif
