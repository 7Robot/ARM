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

void MissionHandler::recv(struct libcan::can_t packet)
{
	printf("MissionHandler::recv::"); fflush(stdout);
	libcan::can_pwrite(1, libcan::dec, &packet);
	std::vector<Mission*>::iterator it;

	if (packet.id == 1028) { // Asserv
		int error = packet.b[0];
		printf("\tAsserv done (error = %d)\n", error);
		for(it =  missions.begin(); it != missions.end(); ++it) {
			if ((*it)->cb_asserv != NULL) {
				((*it)->cb_asserv)->call(&error);
			}
		}
	} else if ((packet.id & 2044) == 256) { // Microswitch
		args_microswitch arg;
		arg.id = (packet.id & 2) >> 1;
		arg.state = (packet.id & 1 == 1);
		printf("\tMicroswitch (id = %d, state = %d)\n", arg.id, arg.state);
		for(it =  missions.begin(); it != missions.end(); ++it) {
			if ((*it)->cb_microswitch != NULL) {
				((*it)->cb_microswitch)->call(&arg);
			}
		}
	} else if ((packet.id & 2012) == 320) { // Sonar
		args_sonar arg;
		arg.id = (packet.id & 32) >> 5;
		arg.edge = (packet.id & 1 == 1);
		arg.value = (packet.b[0] << 8) + packet.b[1];
		printf("\tSonar (id = %d, edge = %d, value = %d)\n", arg.id, arg.edge, arg.value);
		for(it =  missions.begin(); it != missions.end(); ++it) {
			if ((*it)->cb_sonar != NULL) {
				((*it)->cb_sonar)->call(&arg);
			}
		}
	} else {
		// message inconnu
	}
}
