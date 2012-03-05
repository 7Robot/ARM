#include <stdio.h>

#include "Mission.h"

#define SEUIL 6
#define UP	10
#define DOWN	-40

class Evitement: public Mission
{
	void start() {
		can->sonarThres(0, SEUIL * 256);
		msleep(50); /* attente 50 ms */
		can->sonarThres(1, SEUIL * 256);
		msleep(50); /* attente 50 ms */
		can->fwd(50, 50); /* vitesse roue gauche / roue droite (de -80 à 80) */
		state = 1;
	}
	
	bool sonarEvent(int id, bool edge, bool nearby, int value) {
		switch (state) {
			case 1:
				if (id == 0 && nearby) {
					state = 2;
					can->fwd(15, -40);
				} else if (id == 1 && nearby) {
					state = 3;
					can->fwd(-40, 15);
				}
				break;
			case 2:
				if (id == 0 && !nearby) {
					state = 1;
					can->fwd(50, 50);
				}
				break;
			case 3:
				if (id == 1 && !nearby) {
					state = 1;
					can->fwd(50, 50);
				}
				break;
		}

		return true;
	}

	/*void odometryEvent(int x, int y, int theta) {
	}*/
};

extern "C" {
	Mission * create() {
		return new Evitement();
	}
}

