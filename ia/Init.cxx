#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

#define BACK 163
#define LEFT 163

class Init: public Mission
{
	void run() {
		printf("Init::run\n");
		name = "Init";
		state = 1;
		can->fwd(-20, -20);
	}

	bool microswitch(int id, bool status) {
		switch (state) {
			case 1:
				if (id == 0) {
					usleep(700000);
					can->stop();
					state = 2;
					usleep(300000);
					can->fwd(BACK);
				}
				break;
			case 4:
				if (id == 1) {
					usleep(700000);
					can->stop();
					state = 5;
					usleep(300000);
					can->fwd(-LEFT);
				}
				break;
		}
		
		return true;
	}

	bool asserv(int erreur) {
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
				end();
				break;
		}
		
		return true;
	}
};

extern "C" {
	Mission * create() {
		return new Init();
	}
}
