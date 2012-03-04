#include "TaskLoad.h"

#include "../MissionHandler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TaskLoad::TaskLoad(const char * mission, Mission * caller): m_caller(caller)
{
	if (mission != NULL) {
		if ((m_mission = (char*)malloc(strlen(mission) + 1)) == NULL) {
			perror("malloc");
			return;
		}
		strcpy(m_mission, mission);
	}
}

void TaskLoad::exec()
{
	Mission * mission = MissionHandler::load(m_mission);
	if (m_caller != NULL) {
		m_caller->missionLoaded(mission);
	}
}
