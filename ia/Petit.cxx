#include <stdio.h>

#include "Mission.h"

class Petit: public Mission
{
	void run() {
		printf("Petit::run\n");
		state = -2;
		load("init");
		state = -1;
	}

	bool microswitch(int id, bool status) {
		printf("Petit::microswitch: id = %d, status = %d\n", id, status);
		switch (state) {
			case -1:
				if (id == 0) {
					state = 0;
					usleep(100000);
					can->fwd(298);
				}
				break;
			case 7:
				if (id == 1) {
					usleep(300000);
					can->stop();
					usleep(100000);
					can->fwd(-200);
					state = 8;
					//signal();
				}
				break;
			case 52:
				if (id == 1) {
					usleep(300000);
					can->stop();
					usleep(100000);
					can->fwd(-200);
					state = 53;
				}
				break;
		}
	}

	bool asserv(int erreur) {
		printf("Petit::asserv: erreur = %d\n", erreur);

		switch (state) {
			case 0:
				state = 1;
				can->rotate(90);
				break;
			case 1:
				state = 2;
				can->fwd(600);
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
				usleep(2000000);
				can->fwd(30,30);
				break;
			case 8:
				state = 9;
				can->rotate(-90);
				break;
			case 9:
				state = 50;
				can->fwd(1234);
				break;
			case 50:
				state = 51;
				can->rotate(90);
				break;
			case 51:
				state = 52;
				can->fwd(20, 20);
				break;
			case 53:
				state = 54;
				can->rotate(-90);
				break;
			case 54:
				state = 10;
				can->fwd(575);
				break;
			case 10:
				state = 11;
				can->rotate(-90);
				break;
			case 11:
				state = 12;
				can->fwd(1500);
				break;
			case 12:
				state = 13;
				can->rotate(-87);
				break;
			case 13:
				state = 14;
				can->fwd(1700);
				break;
			case 14:
				state = 15;
				can->rotate(180);
				break;
			case 15:
				state = 16;
				load("init");
				signal();
				break;
		}
	}

	bool sonar(int id, bool edge, int value) {
		//printf("Demo::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}

	bool odometry(int x, int y, int theta) {
	}
};

extern "C" {
	Mission * create() {
		return new Petit();
	}
}
