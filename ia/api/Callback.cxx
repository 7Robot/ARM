#include <vector>

#include <stdio.h>

#include "Callback.h"
#include "MissionHandler.h"

class Mission;

void Callback::recv(struct libcan::can_t packet) {
	printf("Callback::recv: "); fflush(stdout);
	libcan::can_pwrite(1, libcan::dec, &packet);

	pthread_mutex_lock(&MissionHandler::mtx); // FIXME

	if (packet.id == 1028) { // Asserv
		int error = packet.b[0];
		printf("\tAsserv done (error = %d)\n", error);
		asserv(error);
    } else if ((packet.id & 2044) == 256) { // Microswitch
        int id = (packet.id & 2) >> 1;
        bool status = (packet.id & 1 == 1);
        printf("\tMicroswitch (id = %d, status = %d)\n", id, status);
		microswitch(id, status);
	} else if ((packet.id & 2012) == 320) { // Sonar
		int id = (packet.id & 32) >> 5;
		bool edge = true;
		bool nearby = (packet.id & 1 == 1);
		int distance = (packet.b[0] << 8) + packet.b[1];
		printf("\tSonar (id = %d, edge = %d, nearby = %d, distance = %d)\n", id, edge, nearby, distance);
		sonar(id, edge, nearby, distance);
	} else if ((packet.id & 2047) == 516) { // Odométrie
        int x = ((int16_t*)packet.b)[0];
        int y = ((int16_t*)packet.b)[1];
        int theta = ((uint16_t*)packet.b)[2];
        printf("\tOdo (x= %.1f cm, y = %.1f cm, theta = %.2f°)\n", x / 10.0, y / 10.0, theta / 100.0);
		odometry(x, y, theta);
    } else {
        // message inconnu
    }

	pthread_mutex_unlock(&MissionHandler::mtx); // FIXME
	
}

void Callback::microswitch(int id, bool status)
{
    std::vector<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		printf("\t[%d]\t%s::microswitch\n", ((*it)->getState)(), ((*it)->getName)());
		if (!((*it)->microswitch)(id, status)) {
			printf("\tCallback propagation stopped\n");
			break;
		}
	}
}

void Callback::asserv(int error)
{
    std::vector<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		printf("\t[%d]\t%s::asserv\n", ((*it)->getState)(), ((*it)->getName)());
		if (!((*it)->asserv)(error)) {
			printf("\tCallback propagation stopped\n");
			break;
		}
	}
}

void Callback::sonar(int id, bool edge, bool nearby, int distance)
{
    std::vector<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		printf("\t[%d]\t%s::sonar\n", ((*it)->getState)(), ((*it)->getName)());
		if (!((*it)->sonar)(id, edge, nearby, distance)) {
			printf("\tCallback propagation stopped\n");
			break;
		}
	}
}

void Callback::odometry(int x, int y, int theta)
{
    std::vector<Mission*>::iterator it;
	for(it =  MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		printf("\t[%3d]\t%s::odometry\n", ((*it)->getState)(), ((*it)->getName)());
		if (!((*it)->odometry)(x, y, theta)) {
			printf("\tCallback propagation stopped\n");
			break;
		}
	}
}
