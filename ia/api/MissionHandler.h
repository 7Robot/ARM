#ifndef _MISSION_HANDLER_H_
#define _MISSION_HANDLER_H_

#include <pthread.h>

#include <set>
#include <string>

#include "Can.h"
#include "libcan.h"

class Mission;

class MissionHandler
{
	public:
		static void setup(const char * basedir, Can * can);
		static Mission * load(const char * mission, Mission * owner);
		static bool unload(Mission * mission);

		static std::set<Mission*> missions;
		static pthread_mutex_t mtx;

	private:
		static const char * basedir;
		static Can * can;
};

//#include "Mission.h"

#endif
