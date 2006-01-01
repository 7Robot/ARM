#ifndef _MISSION_H_
#define _MISSION_H_

#include "Callback.h"
#include "Can.h"

class Mission
{
	public:
		void setup(Can * can);
		virtual void init() = 0;
		virtual void run() = 0;
		
		// Callback functions
		Callback * cb_push_button;
		Callback * cb_asserv;
		Callback * cb_sonar;

	protected:
		Can * can;

};

#endif
