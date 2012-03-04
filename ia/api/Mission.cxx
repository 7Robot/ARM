#include "Mission.h"
#include "Queue.h"
#include "TaskLoad.h"
#include "TaskUnload.h"
#include "TaskSleep.h"

#include <string.h>
#include <unistd.h>

void Mission::setup(Can * can, const char * name) {
	this->can = can;
	if ((this->name = (char *)malloc(strlen(name)+1)) != NULL) {
		strcpy(this->name, name);
	}
}

int Mission::getState() const {
	return state;
}

const char * Mission::getName() const {
	return name;
}

void Mission::start() {}
void Mission::missionLoaded(Mission * mission) {}
bool Mission::missionDone(Mission * mission) { return true; }
bool Mission::microswitchEvent(int id, bool status) { return true; }
bool Mission::asservDone(int error) { return true; }
bool Mission::sonarEvent(int id, bool edge, bool nearby, int distance) { return true; }
bool Mission::odoEvent(int x, int y, int theta) { return true; }
bool Mission::canEvent(struct libcan::can_t packet) { return true; }
void Mission::stop() {}


void Mission::end() {
	Queue::push(new TaskUnload(this));
}

void Mission::load(const char * mission, bool beWarned) {
	printf("Mission::load(%s)\n", mission);
	Queue::push(new TaskLoad(mission, beWarned ? this : NULL));
}

void Mission::unload(Mission * mission) {
	printf("Mission::unload(%s)\n", mission->getName());
	Queue::push(new TaskUnload(mission));
}

void Mission::msleep(int microsecondes)
{
	Queue::push(new TaskSleep(0, microsecondes * 1000));
}

void Mission::sleep(int secondes)
{
	Queue::push(new TaskSleep(secondes));
}
