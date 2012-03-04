#include <vector>

#include <stdio.h>

#include "Spread.h"
#include "MissionHandler.h"
#include "Queue.h"
#include "TaskCanRecv.h"
#include "Mission.h"

int Spread::verbose = 1;

void Spread::packet(struct libcan::can_t packet)
{
	pthread_mutex_lock(&MissionHandler::mtx);

	if (packet.id == 1028) { // Asserv
		int error = packet.b[0];
		printf("\tAsserv done (error = %d)\n", error);
		asservDone(error);
    } else if ((packet.id & 2044) == 256) { // Microswitch
        int id = (packet.id & 2) >> 1;
        bool status = (packet.id & 1 == 1);
        printf("\tMicroswitch (id = %d, status = %d)\n", id, status);
		microswitchEvent(id, status);
	} else if ((packet.id & 2012) == 320) { // Sonar
		int id = (packet.id & 32) >> 5;
		bool edge = true;
		bool nearby = (packet.id & 1 == 1);
		int distance = (packet.b[0] << 8) + packet.b[1];
		printf("\tSonar (id = %d, edge = %d, nearby = %d, distance = %d)\n", id, edge, nearby, distance);
		sonarEvent(id, edge, nearby, distance);
	} else if ((packet.id & 2047) == 516) { // Odométrie
        int x = ((int16_t*)packet.b)[0];
        int y = ((int16_t*)packet.b)[1];
        int theta = ((uint16_t*)packet.b)[2];
        printf("\tOdo (x= %.1f cm, y = %.1f cm, theta = %.2f°)\n", x / 10.0, y / 10.0, theta / 100.0);
		odoEvent(x, y, theta);
    } else {
		canEvent(&packet);
    }

	pthread_mutex_unlock(&MissionHandler::mtx);
}

void Spread::missionDone(Mission * mission)
{
	printf("Spread::missionDone: %s\n", mission == NULL ? "(null)" : mission->getName());

	std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->missionDone)(mission);
		if (verbose > 0 || state != ((*it)->getState)()) {
			printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::microswitchEvent(int id, bool status)
{
    std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->microswitchEvent)(id, status);
		if (verbose > 0 || state != ((*it)->getState)()) {
			printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::asservDone(int error)
{
    std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->asservDone)(error);
		if (verbose > 0 || state != ((*it)->getState)()) {
			printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::sonarEvent(int id, bool edge, bool nearby, int distance)
{
    std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->sonarEvent)(id, edge, nearby, distance);
		if (verbose > 0 || state != ((*it)->getState)()) {
			printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::odoEvent(int x, int y, int theta)
{
    std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->odoEvent)(x, y, theta);
		if (verbose > 1 || state != ((*it)->getState)()) {
			printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::canEvent(struct libcan::can_t * packet)
{
	printf("\tCall canEvent ...\n");

    std::set<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int state = ((*it)->getState)();
		bool r = ((*it)->canEvent)(*packet);
		if (verbose > 0 || state != ((*it)->getState)()) {
			//printf("\t[%3d]->[%3d]\t%s\n", state, ((*it)->getState)(), ((*it)->getName)());
		}
		if (!r) {
			//printf("\tPropagation stopped\n");
			break;
		}
	}
}
