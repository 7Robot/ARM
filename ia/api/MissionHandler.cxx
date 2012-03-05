#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include "MissionHandler.h"
#include "Mission.h"
#include "Queue.h"
#include "TaskLoad.h"

std::set<Mission*> MissionHandler::missions;
pthread_mutex_t MissionHandler::mtx = PTHREAD_MUTEX_INITIALIZER;
const char * MissionHandler::basedir;
Can * MissionHandler::can;

void MissionHandler::setup(const char * basedir, Can * can)
{
	MissionHandler::basedir = basedir;
	MissionHandler::can = can;
}

Mission * MissionHandler::load(const char * name, Mission * owner)
{
	printf("MissionHandler::load(%s)\n", name);

	if (name == NULL) {
		return NULL;
	}

	void *hndl;
	char path[256];

	// try "libname.so"
	strcpy(path, "lib");
	strcat(path, name);
	strcat(path, ".so");
	hndl = dlopen(path, RTLD_LAZY);
	if(hndl == NULL) {
		// try "dir/libname.so"
		strcpy(path, MissionHandler::basedir);
		strcat(path, "lib");
		strcat(path, name);
		strcat(path, ".so");
		hndl = dlopen(path, RTLD_LAZY);
		if (hndl == NULL) {
			fprintf(stderr, "dlopen failed(%s)\n", dlerror());
			return NULL;
		}
	}

	void * create = dlsym(hndl, "create");
	if (create == NULL) {
		fprintf(stderr, "dlsym failed (%s)\n", dlerror());
		return NULL;
	}
	
	Mission * mission = ((Mission*(*)())create)();
	strcpy(path, name);
	path[0] = path[0] - ' ';
	mission->setup(can, path, owner);

	pthread_mutex_lock(&MissionHandler::mtx);
	missions.insert(mission);
	printf("Running missions: %d [+1: %s]\n", missions.size(), path);
	pthread_mutex_unlock(&MissionHandler::mtx);

	printf("%s::start\n", path);
	mission->start();

	return mission;
}

bool MissionHandler::unload(Mission * mission)
{
	printf("MissionHandler::unload(%s)\n", mission->getName());

	pthread_mutex_lock(&MissionHandler::mtx);
	mission->stop();
	MissionHandler::missions.erase(mission);
	printf("Running missions: %d [-1: %s]\n", missions.size(), mission->getName());
	pthread_mutex_unlock(&MissionHandler::mtx);

	return false;
}
