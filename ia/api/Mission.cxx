#include "Mission.h"
#include "Queue.h"
#include "TaskLoad.h"
#include "TaskUnload.h"

#include <string.h>

void Mission::setup(Can * can, const char * name)
{
	this->can = can;
	if ((this->name = (char *)malloc(strlen(name)+1)) != NULL) {
		strcpy(this->name, name);
	}
}

int Mission::getState() const
{
	return state;
}

const char * Mission::getName() const
{
	return name;
}

void Mission::end()
{
	Queue::push(new TaskUnload(this));
}

void Mission::load(const char * mission)
{
	printf("Mission::load(%s)\n", mission);
	Queue::push(new TaskLoad(mission));
}
