#ifndef _MISSION_HANDLER_H_
#define _MISSION_HANDLER_H_

#include <vector>
#include <string>

#include "Can.h"
#include "libcan.h"

class Mission;

class MissionHandler
{
	public:
		MissionHandler(char * basedir, Can * can);
		bool load(const char * mission);
		static std::vector<Mission*> missions;

	private:
		char * basedir;
		Can * can;
};

#include "Mission.h"

#endif
