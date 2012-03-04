#ifndef _SPREAD_H_
#define _SPREAD_H_

#include "libcan.h"
#include "Can.h"
#include "Mission.h"

class Spread
{
	public:
		static void packet(struct libcan::can_t packet);
		static void missionDone(Mission * mission);
	
	private:
		static void microswitchEvent(int id, bool status);
		static void asservDone(int error);
		static void sonarEvent(int id, bool edge, bool nearby, int distance);
		static void odoEvent(int x, int y, int theta);
		static void canEvent(struct libcan::can_t * packet);

		static int verbose;

};

#endif
