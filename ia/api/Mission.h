#ifndef _MISSION_H_
#define _MISSION_H_

#include "Callback.h"
#include "Can.h"

class MissionHandler;

class Mission
{
	public:
		void setup(MissionHandler * mh, Can * can);
		virtual void init() = 0;
		virtual void run() = 0;
		
		// Callback functions
		Callback * cb_microswitch;
		Callback * cb_asserv;
		Callback * cb_sonar;

	protected:
		MissionHandler * mh;
		Can * can;

};

#endif
