#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include "MissionHandler.h"

std::vector<Mission*> MissionHandler::missions;

MissionHandler::MissionHandler(char * basedir, Can * can)
{
	this->basedir = basedir;
	this->can = can;
}

bool MissionHandler::load(const char * name)
{
	printf("MissionHandler::load(%s)\n", name);

	void *hndl;
	char path[256];

	// try "libname.so"
	strcat(path, "lib");
	strcat(path, name);
	strcat(path, ".so");
	hndl = dlopen(path, RTLD_LAZY);
	if(hndl == NULL) {
		// try "name"
		hndl = dlopen(name, RTLD_LAZY);
		if (hndl == NULL) {
			// try "dir/libname.so"
			strcpy(path, basedir);
			strcat(path, "lib");
			strcat(path, name);
			strcat(path, ".so");
			hndl = dlopen(path, RTLD_LAZY);
			if (hndl == NULL) {
				fprintf(stderr, "ldopen failed(%s)\n", dlerror());
				return true;
			}
		}
	}

	void * create = dlsym(hndl, "create");
	if (create == NULL) {
		fprintf(stderr, "dlsym failed (%s)\n", dlerror());
		return true;
	}

	Mission * mission = ((Mission*(*)())create)();
	mission->setup(this, can);
	
	missions.push_back(mission);
	printf("Running missions: %d [+1]\n", missions.size());
	
	mission->init();

	missions.pop_back();
	printf("Running missions: %d [-1]\n", missions.size());

	return false;
}
