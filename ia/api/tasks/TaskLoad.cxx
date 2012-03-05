#include "TaskLoad.h"

#include "../MissionHandler.h"
#include "../Spread.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TaskLoad::TaskLoad(const char * mission, Mission * owner): m_owner(owner)
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
	Mission * mission = MissionHandler::load(m_mission, m_owner);
	if (mission == NULL) {
		if (m_owner != NULL) {
			m_owner->missionLoaded(mission, true);
		}
	} else {
		Spread::missionLoaded(mission, m_owner);
	}
}
