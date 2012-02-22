#include <stdio.h>
#include <unistd.h>

#include "Mission.h"
#include "Callback.h"

class Demo: public Mission
{
	private:
	
	int state;

	public:

	void init() {
		printf("Demo::init()\n");

		this->cb_asserv = new CB_Asserv<Demo>(this, &Demo::asserv);
		this->cb_microswitch = new CB_Microswitch<Demo>(this, &Demo::microswitch);
	}

	void run() {
		printf("Demo::run\n");
		can->forward(0);
		usleep(100000);
		state = 1;
		can->forward(0);
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
				can->forward(longueur);
				state = 2;
				break;
			case 2:
				usleep(delai);
				can->forward(-longueur);
				state = 3;
				break;
			case 3:
				usleep(delai);
				can->rotate(90);
				state = 4;
				break;
			case 4:
				usleep(delai);
				can->forward(longueur);
				state = 5;
				break;
			case 5:
				usleep(delai);
				can->forward(-longueur);
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
