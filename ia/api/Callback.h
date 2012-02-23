#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "libcan.h"
#include "Can.h"

class Callback
{
	public:
		static void recv(struct libcan::can_t packet);
	
	private:
		static void microswitch(int id, bool status);
		static void asserv(int error);
		static void sonar(int id, bool edge, bool nearby, int distance);
		static void odometry(int x, int y, int theta);

};

#endif
