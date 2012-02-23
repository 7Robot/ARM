#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Init: public Mission
{
	void init() {
		printf("Init::init()\n");
	}

	void run() {
		printf("Init::run\n");
		state = 1;
		can->fwd(-20, -20);
		wait();
	}

	void microswitch(int id, bool status) {
		printf("Init::microswitch: id = %d, state = %d\n", id, status);

		switch (state) {
			case 1:
				if (id == 0) {
					usleep(700000);
					can->stop();
					state = 2;
					usleep(300000);
					can->fwd(200);
				}
				break;
			case 4:
				if (id == 1) {
					usleep(700000);
					can->stop();
					state = 5;
					usleep(300000);
					can->fwd(-150);
				}
				break;
		}
	}

	void asserv(int erreur) {
		printf("Init::asserv: erreur = %d\n", erreur);

		switch (state) {
			case 2:
				state = 3;
				can->rotate(-90);
				break;
			case 3:
				state = 4;
				can->fwd(20, 20);
				break;
			case 5:
				state = 6;
				can->rotate(90);
				break;
			case 6:
				can->odoReset();
				signal();
				break;
		}
	}
};

extern "C" {
	Mission * create() {
		return new Init();
	}
}
