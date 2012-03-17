#include "Module.h"

#include "Recv.h"
#include "Send.h"

#include <stdio.h>

#define BACK 163
#define LEFT 163


class Recalage: public Module
{	
	void init() {
		BIND1(asserv::done, Recalage::asservDone);
		BIND2(captor::microswitch, Recalage::microswitchEvent);

		state = 1;
		send(asserv::rotate(90));
	}

	bool microswitchEvent(int id, bool status) {
		//printf("microswitchEvent(%d, %d)\n", id, status);
		switch (state) {
			case 2:
				if (id == 0 && status) {
					state = 3;
					send(asserv::stop())->setDelay(700);
					send(asserv::fwd(LEFT))->setDelay(1000);
				}
				break;
			case 5:
				if (id == 0 && status) {
					state = 6;
					send(asserv::stop())->setDelay(700);
					send(asserv::fwd(BACK))->setDelay(1000);
				}
				break;
		}
	}

	void asservDone(int error) {
		//printf("asservDone %d\n", error);
		switch (state) {
			case 1:
				state = 2;
				send(asserv::fwd(-20, -20));
				break;
			case 3:
				state = 4;
				send(asserv::rotate(-90));
				break;
			case 4:
				state = 5;
				send(asserv::fwd(-20, -20));
				break;
			case 6:
				abord();
				break;
		}
	}

};

extern "C" {
	Module * create() {
		return new Recalage();
	}
}
