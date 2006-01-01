#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include "MissionHandler.h"
#include "Mission.h"

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

	strcpy(path, "lib");
	strcat(path, name);
	strcat(path, ".so");
	
	hndl = dlopen(path, RTLD_LAZY);
	if(hndl == NULL) {
		strcpy(path, basedir);
		strcat(path, "lib");
		strcat(path, name);
		strcat(path, ".so");
		hndl = dlopen(path, RTLD_LAZY);
		if (hndl == NULL) {
			fprintf(stderr, "ldopen failed(%s)\n", dlerror());
			return false;
		}
	}

	void * create = dlsym(hndl, "create");
	if (create == NULL) {
		fprintf(stderr, "dlsym failed (%s)\n", dlerror());
		return false;
	}

	Mission * mission = ((Mission*(*)())create)();
	mission->setup(can);
	mission->init();
	mission->run();

	return true;
}

void MissionHandler::recv(struct libcan::can_t packet)
{
	libcan::can_pwrite(1, libcan::dec, &packet);
}

