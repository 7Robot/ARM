#ifndef _MISSION_HANDLER_H_
#define _MISSION_HANDLER_H_

#include <pthread.h>

#include <vector>
#include <string>

#include "Can.h"
#include "libcan.h"

class Mission;

class MissionHandler
{
	public:
		//MissionHandler(char * basedir, Can * can);
		static void setup(const char * basedir, Can * can);
		static bool handler(const char * init);
		static bool load(const char * mission);
		static bool unload();
		static std::vector<Mission*> missions;
		static pthread_mutex_t mtx;
		static pthread_mutex_t cnd_mtx;
		static pthread_cond_t cnd;
		static void wait();
		static void signal();
		static char * next;
		static bool unloadinprogress;

	private:
		static const char * basedir;
		static Can * can;
};

#include "Mission.h"

#endif
