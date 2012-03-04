#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "libcan.h"
#include "Can.h"

class Callback
{
	public:
		static void packet(struct libcan::can_t packet);
	
	private:
		static void missionDone();
		static void microswitchEvent(int id, bool status);
		static void asservDone(int error);
		static void sonarEvent(int id, bool edge, bool nearby, int distance);
		static void odometryEvent(int x, int y, int theta);

};

#endif
