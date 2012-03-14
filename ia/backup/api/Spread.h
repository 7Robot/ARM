#ifndef _SPREAD_H_
#define _SPREAD_H_

#include "libcan.h"
#include "Can.h"
#include "Mission.h"

class Spread
{
	public:
		static void packet(struct libcan::can_t packet);
		static void missionLoaded(Mission * mission, Mission * owner);
		static void missionDone(Mission * mission, Mission * applicant);
	
	private:
		template <typename T> static void spread(T caller);

		static int verbose;

};

#endif
