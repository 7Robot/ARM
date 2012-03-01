#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Str: public Mission
{
	void run() {
		printf("Str::run\n");
		name = "Str";
		state = 1;
		load("fwd");
	}

	void mission() {
		switch (state) {
			case 1:
				can->fwd(200);
				state = 2;
				break;
			case 3:
				can->rotate(90);
				state = 4;
				break;
			case 5:
				load("fwd");
				state = 6;
				break;
			case 6:
				end();
				break;
		}
	}

	bool asserv(int erreur) {
		switch (state) {
			case 2:
				state = 3;
				load("fwd");
				break;
			case 4:
				load("fwd");
				state = 5;
				break;
		}	
		return true;
	}
};

extern "C" {
	Mission * create() {
		return new Str();
	}
}
