#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Fwd: public Mission
{
	void start() {
		state = 1;
		can->fwd(150);
	}

	bool asservDone(int erreur) {
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
