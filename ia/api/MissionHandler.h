#ifndef _MISSION_HANDLER_H_
#define _MISSION_HANDLER_H_

#include <vector>

#include "Mission.h"
#include "Can.h"
#include "libcan.h"

class MissionHandler
{
	public:
		MissionHandler(char * basedir, Can * can);
		bool load(char * mission);
		static std::vector<Mission*> missions;

	private:
		char * basedir;
		Can * can;
};

#endif
