#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Fwd: public Mission
{
	void run() {
		printf("Fwd::run\n");
		name = "Fwd";
		state = 1;
		can->fwd(150);
	}

	bool asserv(int erreur) {
		switch (state) {
			case 1:
				end();
				break;
		}
		
		return true;
	}
};

extern "C" {
	Mission * create() {
		return new Fwd();
	}
}
