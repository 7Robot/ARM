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

bool MissionHandler::load(char * name)
{
	printf("MissionHandler::load(%s)\n", name);

	void *hndl;
	char path[256];

	hndl = dlopen(name, RTLD_LAZY);
	if(hndl == NULL) {
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

	void * create = dlsym(hndl, "create");
	if (create == NULL) {
		fprintf(stderr, "dlsym failed (%s)\n", dlerror());
		return true;
	}

	Mission * mission = ((Mission*(*)())create)();
	mission->setup(this, can);
	mission->init();
	
	missions.push_back(mission);
	printf("Running missions: %d [+1]\n", missions.size());
	
	mission->run();

	missions.pop_back();
	printf("Running missions: %d [-1]\n", missions.size());

	return false;
}
