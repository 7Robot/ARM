#include <stdio.h>

#include "Mission.h"

class Petit: public Mission
{
	void init() {
		printf("Petit::init()\n");
		mh->load("init");
	}

	void run() {
		printf("Petit::run\n");
		state = 0;
		can->fwd(200);
		wait();
	}

	void microswitch(int id, bool status) {
		printf("Petit::microswitch: id = %d\n", id);
		//printf("statengo::microswitch: id = %d, state = %d\n", id, state);
		switch (state) {
			case 7:
				if (id == 1) {
					can->stop();
					signal();
				}
				break;
		}
	}

	void asserv(int erreur) {
		printf("Petit::asserv: erreur = %d\n", erreur);

		switch (state) {
			case 0:
				state = 1;
				can->rotate(90);
				break;
			case 1:
				state = 2;
				can->fwd(800);
				break;
			case 2:
				state = 3;
				can->rotate(90);
				break;
			case 3:
				state = 4;
				can->fwd(400);
				break;
			case 4:
				state = 5;
				can->fwd(-400);
				break;
			case 5:
				state = 6;
				can->rotate(-90);
				break;
			case 6:
				state = 7;
				can->fwd(80,80);
				break;
		}
	}

	void sonar(int id, bool edge, int value) {
		//printf("Demo::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}

	void odometry(int x, int y, int theta) {
	}
};

extern "C" {
	Mission * create() {
		return new Petit();
	}
}
