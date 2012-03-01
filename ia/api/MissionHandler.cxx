#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#include "MissionHandler.h"

std::vector<Mission*> MissionHandler::missions;
pthread_mutex_t MissionHandler::mtx;
pthread_mutex_t MissionHandler::cnd_mtx;
pthread_cond_t MissionHandler::cnd;
const char * MissionHandler::basedir;
Can * MissionHandler::can;
char * MissionHandler::next;
bool MissionHandler::unloadinprogress;

void MissionHandler::setup(const char * basedir, Can * can)
{
	MissionHandler::basedir = basedir;
	MissionHandler::can = can;
	MissionHandler::mtx = PTHREAD_MUTEX_INITIALIZER;
	MissionHandler::cnd_mtx = PTHREAD_MUTEX_INITIALIZER;
	MissionHandler::cnd = PTHREAD_COND_INITIALIZER;
	MissionHandler::next = NULL;
	MissionHandler::unloadinprogress = false;
}

bool MissionHandler::handler(const char * init)
{
	char * n = NULL;
	load(init);
	while (true) {
		while (next != NULL && strcmp(next, "unload")) {
			n = next;
			next = NULL;
			load(n);
		}
		wait();
		while (next != NULL && !strcmp(next, "unload")) {
			next = NULL;
			unload();
		}
	}
/*	while (!missions.empty()) {
		if (next == NULL) {
			//MissionHandler::unloadinprogress = true;
			unload();
			//MissionHandler::unloadinprogress = false;
		}
		if (next != NULL) {
			load(next);
			next = NULL;
		}
		wait();
	}*/

	return false;
}

void MissionHandler::wait()
{
    printf("MissionHandler::wait\n");

    pthread_mutex_lock(&cnd_mtx);
    pthread_cond_wait(&cnd, &cnd_mtx);
	pthread_mutex_unlock(&cnd_mtx);

    printf("MissionHandler::wait: wakeup !\n");
}

void MissionHandler::signal()
{
    printf("MissionHandler::signal\n");

    pthread_mutex_lock(&cnd_mtx);
    //printf("MissionHandler::signal: send ... !\n");
    pthread_cond_signal(&cnd);
    //printf("MissionHandler::signal: unlock ... !\n");
    pthread_mutex_unlock(&cnd_mtx);

    //printf("MissionHandler::signal: sended !\n");
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
			strcpy(path, MissionHandler::basedir);
			strcat(path, "lib");
			strcat(path, name);
			strcat(path, ".so");
			hndl = dlopen(path, RTLD_LAZY);
			if (hndl == NULL) {
				fprintf(stderr, "dlopen failed(%s)\n", dlerror());
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
	mission->setup(can);

	printf("lock\n");
	pthread_mutex_lock(&MissionHandler::mtx); // FIXME
	printf("lock ok\n");
	missions.push_back(mission);
	pthread_mutex_unlock(&MissionHandler::mtx); // FIXME
	
	printf("Running missions: %d [+1]\n", missions.size());

	// TODO parse name
	mission->run();

	return false;
}

bool MissionHandler::unload()
{
	printf("MissionHandler::unload\n");

	Mission * mission = NULL;
	
	pthread_mutex_lock(&MissionHandler::mtx); // FIXME
	//printf("MissionHandler::unload:pop_back()\n");
	missions.pop_back();
	if (!missions.empty()) {
		mission = missions.back();
	}
	if (mission != NULL) { // FIXME FIXME FIXME
		printf("\t[%d]\t%s::mission\n", (missions.back())->getState(), (missions.back())->getName());
		mission->mission();
	}
	pthread_mutex_unlock(&MissionHandler::mtx); // FIXME
	
	printf("Running missions: %d [-1]\n", missions.size());

	return false;
}
