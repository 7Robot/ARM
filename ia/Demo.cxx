#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Demo: public Mission
{
	void init() {
		printf("Demo::init()\n");
		mh->load("init");
	}

	void run() {
		printf("Demo::run\n");
		can->fwd(0);
		usleep(100000);
		state = 1;
		can->fwd(0);
		pause();
	}

	void microswitch(int id, bool state) {
		printf("Demo::microswitch: id = %d, state = %d\n", id, state);
	}

	void asserv(int erreur) {
		printf("Demo::asserv: erreur = %d\n", erreur);

		int delai = 100000;
		int longueur = 1000;
		switch (state) {
			case 1:
				usleep(delai);
				can->fwd(longueur);
				state = 2;
				break;
			case 2:
				usleep(delai);
				can->fwd(-longueur);
				state = 3;
				break;
			case 3:
				usleep(delai);
				can->rotate(90);
				state = 4;
				break;
			case 4:
				usleep(delai);
				can->fwd(longueur);
				state = 5;
				break;
			case 5:
				usleep(delai);
				can->fwd(-longueur);
				state = 6;
				break;
			case 6:
				usleep(delai);
				can->rotate(-90);
				state = 1;
				break;
		}
	}

	void sonar(int id, bool edge, int value) {
		printf("Demo::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}
};

extern "C" {
	Mission * create() {
		return new Demo();
	}
}
