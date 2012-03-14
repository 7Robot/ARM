#include "TaskUnload.h"

#include "../MissionHandler.h"
#include "../Spread.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TaskUnload::TaskUnload(Mission * mission, Mission * applicant):
	m_mission(mission), m_applicant(applicant) {}

void TaskUnload::exec()
{
	MissionHandler::unload(m_mission);
	Spread::missionDone(m_mission, m_applicant);
	delete m_mission;
}
