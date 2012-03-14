#include <vector>

#include <stdio.h>

#include "Spread.h"
#include "MissionHandler.h"
#include "Queue.h"
#include "TaskCanRecv.h"
#include "Mission.h"
#include "libcan.h"

int Spread::verbose = 1;

template <typename T>
void Spread::spread(T caller){
	for (auto it = MissionHandler::missions.begin(); it != MissionHandler::missions.end(); ++it) {
		int oldState = ((*it)->getState)();
		bool r = caller(*it);
		int newState = ((*it)->getState)();
		if (verbose > 0 || oldState != newState) {
			printf("\t[%3d]->[%3d]\t%s\n", oldState, newState, ((*it)->getName)());
		}
		if (!r) {
			printf("\tPropagation stopped\n");
			break;
		}
	}
}

void Spread::packet(struct libcan::can_t packet)
{
	printf("Can::recv: "); fflush(stdout);
	can_pwrite(1, dec, &packet);

	pthread_mutex_lock(&MissionHandler::mtx);

    if ((packet.id & 2044) == 256) { // Microswitch state changed
        int id = (packet.id & 2) >> 1;
        bool status = (packet.id & 1 == 1);
        printf("\tMicroswitch (id = %d, status = %d)\n", id, status);
		auto caller = [&id,&status](Mission * mission)->bool {
			return (mission->microswitchEvent)(id, status);
		};
		spread<decltype(caller)>(caller);
	} else if ((packet.id & 2012) == 320) { // Sonar answer
		int id = (packet.id & 32) >> 5;
		bool edge = true;
		bool nearby = (packet.id & 1 == 1);
		int distance = (packet.b[0] << 8) + packet.b[1];
		printf("\tSonar (id = %d, edge = %d, nearby = %d, distance = %d)\n", id, edge, nearby, distance);
		auto caller = [&id,&edge,&nearby,&distance](Mission * mission)->bool {
			return (mission->sonarAnswer)(id, edge, nearby, distance);
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 513) { // Odometry request
		printf("\tOdometry request\n");
		auto caller = [](Mission * mission)->bool {
			return (mission->odoRequest)();
		};
		spread<decltype(caller)>(caller);
	} else if ((packet.id & 2046) == 514) { // Odometry : broadcast/mute
		bool broadcast = (packet.id & 1) == 1; // broadcast or mute ?
		printf("\tOdometry broadcast %s\n", broadcast?"enabled":"disabled");
		auto caller = [&broadcast](Mission * mission)->bool {
			return (mission->odoBroadcast)(broadcast);
		};
		spread<decltype(caller)>(caller);
	} else if ((packet.id & 2046) == 516) { // Odometry : set position/position answer
		int x = ((int16_t*)packet.b)[0];
		int y = ((int16_t*)packet.b)[1];
        int theta = ((uint16_t*)packet.b)[2];
		bool set = (packet.id & 1) == 1; // Set or answer ?
        printf("\tOdo %s values : x = %.1f cm, y = %.1f cm, theta = %.2f°\n", set?"set":"answer", x / 10.0, y / 10.0, theta / 100.0);
		std::function<bool (Mission *)> caller;
		if (set) {
			caller = [&x, &y, &theta](Mission * mission)->bool {
				return (mission->odoSet)(x, y, theta);
			};
		} else {
			caller = [&x, &y, &theta](Mission * mission)->bool {
				return (mission->odoAnswer)(x, y, theta);
			};
		}
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1025) { // Asserv line order
		int distance = *((int16_t*)packet.b) / 13.4; // FIXME
		printf("\tAsserv line order : distance = %d mm\n", distance);
		auto caller = [&distance](Mission * mission)->bool {
			return (mission->asservLineOrder)(distance);
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1026) { // Asserv rotation order
		int angle = *((int16_t*)packet.b) / 0.234; // FIXME
		printf("\tAsserv rotation order : angle = %.2f° (%d)\n", angle/100.0, *((int16_t*)packet.b));
		auto caller = [&angle](Mission * mission)->bool {
			return (mission->asservRotOrder)(angle);
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1028) { // Asserv done
		int error = packet.b[0];
		printf("\tAsserv done (error = %d)\n", error);
		auto caller = [&error](Mission * mission)->bool {
			return (mission->asservDone)(error);
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1051) { // Asserv stop
		printf("\tAsserv stop\n");
		auto caller = [](Mission * mission)->bool {
			return (mission->asservStop)();
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1029) { // Asserv speed order without ramp
		printf("\tAsserv speed order without ramp : left = %d, right = %d\n",
				(int8_t)packet.b[0], (int8_t)packet.b[1]);
		auto caller = [&packet](Mission * mission)->bool {
			return (mission->asservSpeedOrder((int8_t)packet.b[0],
						(int8_t)packet.b[1], false)); // no ramAp
		};
		spread<decltype(caller)>(caller);
	} else if (packet.id == 1032) { // Asserv speed order with ramp
		printf("\tAsserv speed order with ramp : left = %d, right = %d\n", (int8_t)packet.b[0], (int8_t)packet.b[1]);
		auto caller = [&packet](Mission * mission)->bool {
			return (mission->asservSpeedOrder((int8_t)packet.b[0], (int8_t)packet.b[1], true)); // ramp
		};
		spread<decltype(caller)>(caller);
	} else if ((packet.id & 2046) == 1030) { // Set asserv up/down
		bool up = (packet.id & 1) == 0; // Up or down ?
		printf("\tAsserv %s\n", up?"up":"down");
		auto caller = [&up](Mission * mission)->bool {
			return (mission->asservUp)(up);
		};
		spread<decltype(caller)>(caller);
	} else if ((packet.id & 2047) == 127) { // Global reset
		printf("\tGlobal reset\n");
		auto caller = [](Mission * mission)->bool {
			return (mission->globalReset)();
		};
		spread<decltype(caller)>(caller);
    } else {
		printf("\tUnknow packet\n");
		auto caller = [&packet](Mission * mission)->bool {
			return (mission->canEvent)(packet);
		};
		spread<decltype(caller)>(caller);
    }

	pthread_mutex_unlock(&MissionHandler::mtx);
}

void Spread::missionLoaded(Mission * mission, Mission * owner)
{
	printf("Spread::missionLoaded\n");

	auto caller = [&mission,&owner](Mission * it)->bool {
		return (mission->missionLoaded)(mission, it == owner);
	};
	spread<decltype(caller)>(caller);
}

void Spread::missionDone(Mission * mission, Mission * applicant)
{
	printf("Spread::missionDone\n");

	auto caller = [&mission,&applicant](Mission * it)->bool {
		return (mission->missionDone)(mission, it == mission->getOwner(), mission == applicant);
	};
	spread<decltype(caller)>(caller);
}
