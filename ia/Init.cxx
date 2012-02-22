#include <stdio.h>
#include <unistd.h>

//#include "Init.h"
#include "Callback.h"
#include "Mission.h"

class Init: public Mission
{
	private:
	
	int state;

	public:
	
	void init() {
		printf("Demo::init()\n");
		//this->cb_asserv = new CB_Asserv<Demo>(this, &Demo::asserv);
		//this->cb_microswitch = new CB_Microswitch<Demo>(this, &Demo::microswitch);
	}

	void run() {
		printf("Demo::run\n");
		//can->forward(0);
		usleep(50000);
		//can->forward(50);
		state = 2;
		pause();
	}
};

/*void Demo::microswitch(int id, bool state)
{
	printf("Demo::microswitch: id = %d, state = %d\n", id, state);
}

void Demo::asserv(int erreur)
{
	printf("Demo::asserv: erreur = %d\n", erreur);

	int delai = 5000;
	switch (state) {
		case 1:
			usleep(delai);
			can->forward(200);
			state = 2;
			break;
		case 2:
			usleep(delai);
			can->rotate(90);
			state = 3;
			break;
		case 3:
			usleep(delai);
			can->forward(-200);
			state = 4;
			break;
		case 4:
			usleep(delai);
			can->forward(200);
			state = 5;
			break;
		case 5:
			usleep(delai);
			can->rotate(-90);
			state = 6;
			break;
		case 6:
			usleep(delai);
			can->forward(-200);
			state = 1;
			break;
	}
}

void Demo::sonar(int id, bool edge, int value)
{
	printf("Demo::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
}*/

extern "C" {
	Mission * create() {
		return new Init();
	}
}
