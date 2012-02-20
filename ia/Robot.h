#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "CallbackHandler.h"
#include "CanHandler.h"
#include <libcan.h>

class Robot {

	public:
		Robot(int canbus);
		void start();
		static void recv(libcan::can_t packet);
		CallbackHandler cbh;
		CanHandler * ch;

	private:
		int canbus;

};

extern Robot * robot;

#endif
