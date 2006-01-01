#ifndef _MISSION_HANDLER_H_
#define _MISSION_HANDLER_H_

#include "Can.h"
#include "libcan.h"

class MissionHandler
{
	public:
		MissionHandler(char * basedir, Can * can);
		bool load(char * mission);
		static void recv(struct libcan::can_t packet);

	private:
		char * basedir;
		Can * can;
};

#endif
