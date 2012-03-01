#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

#define BACK 163
#define LEFT 163

class Recalage: public Mission
{
	void run() {
		printf("Recalage::run\n");
		name = "Recalage";
		state = 1;
		can->rotate(90);
	}

	bool microswitch(int id, bool status) {
		switch (state) {
			case 2:
				if (id == 0 && status) {
					usleep(700000);
					can->stop();
					state = 3;
					usleep(300000);
					can->fwd(LEFT);
				}
				break;
			case 5:
				if (id == 0 && status) {
					usleep(700000);
					can->stop();
					state = 6;
					usleep(300000);
					can->fwd(BACK);
				}
				break;
		}
		
		return true;
	}

	bool asserv(int erreur) {
		switch (state) {
			case 1:
				state = 2;
				can->fwd(-20, -20);
				break;
			case 3:
				state = 4;
				can->rotate(-90);
				break;
			case 4:
				state = 5;
				can->fwd(-20, -20);
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
		return new Recalage();
	}
}
