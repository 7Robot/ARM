#include "Mission.h"
#include "Queue.h"
#include "TaskLoad.h"
#include "TaskUnload.h"
#include "TaskSleep.h"

#include <string.h>
#include <unistd.h>

void Mission::setup(Can * can, const char * name, Mission * owner) {
	this->can = can;
	if ((this->name = (char *)malloc(strlen(name)+1)) != NULL) {
		strcpy(this->name, name);
	}
	this->owner = owner;
}

int Mission::getState() const {
	return state;
}

const char * Mission::getName() const {
	return name;
}

Mission * Mission::getOwner() const {
	return owner;
}

void Mission::start() {}

bool Mission::missionLoaded(Mission * mission, bool ownMission) {
	if (ownMission) {
		return missionLoaded(mission);
	} else {
		printf(" (ign)");
		return true;
	}
}
bool Mission::missionLoaded(Mission * mission) {
	return true;
}

bool Mission::missionDone(Mission * mission, bool ownMission, bool completed) {
	if (ownMission && completed) {
		return missionDone(mission);
	} else {
		printf(" (ign)");
		return true;
	}
}
bool Mission::missionDone(Mission * mission) {
	return true;
};

void Mission::stop() {}

void Mission::end() {
	Queue::push(new TaskUnload(this, this));
}

void Mission::load(const char * mission) {
	printf("Mission::load(%s)\n", mission);
	Queue::push(new TaskLoad(mission, this));
}

void Mission::unload(Mission * mission) {
	printf("Mission::unload(%s)\n", mission->getName());
	Queue::push(new TaskUnload(mission, this));
}


/*void Mission::msleep(int microsecondes)
{
	Queue::push(new TaskSleep(0, microsecondes * 1000));
}*/

/*void Mission::sleep(int secondes)
{
	Queue::push(new TaskSleep(secondes));
}*/
