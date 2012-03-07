#include <stdio.h>

#include "Mission.h"

//TODO incrémentation auto ?
// États définis :
#define RECALAGE     	    -2
#define ATTENTE_DEPART	    -1
#define DEPART              0
#define TOURNE_BOUTEILLE_1	1
#define AXE_BOUTEILLE_1   	2
#define ATTENTE_BOUTEILLE_1 71
//TODO incrémentation auto ?

class Petit: public Mission
{
	void start() {
		state = RECALAGE;
		can->odoReset();
		load("recalage");
	}

	bool missionDone(Mission * mission, bool ownMission, bool completed)
	{
		printf("plop\n");
		if (!ownMission || !completed) {
			return true;
		}

		switch (state) {
			case RECALAGE:
				state = ATTENTE_DEPART;
				break;
			case 16:
				state = 17;
				load("evitement");
				break;
			case 17:
				end();
				break;
		}

		return true;
	}

	bool microswitchEvent(int id, bool status) {
		switch (state) {
			case ATTENTE_DEPART:
				if (id == 0) { // Laisse de démarrage.
					state = DEPART;
					can->fwd(395)->setDelay(100);
				}
				break;
			case ATTENTE_BOUTEILLE_1: // TODO déclencher aussi par timer
				if (id == 1) {
					can->stop()->setDelay(300);
					can->fwd(-200)->setDelay(100); // On s'éloigne.
					state = 8; /// Mais il veut pas s'arrêter !!!
				}
				break;
			case 52:
				if (id == 1) {
					can->stop()->setDelay(300);
					can->fwd(-200)->setDelay(100);
					state = 53;
				}
				break;
		}

		return true;
	}

	bool asservDone(int erreur) {

		switch (state) {
			case DEPART:
				state = 1; 
				can->rotate(90);
				break;
			case 1: // On avance en direction de la bouteille.
				state = 2;
				can->fwd(600);
				break;
			case 2: // Pause lingot.
				state = 3;
				can->rotate(90);
				break;
			case 3:
				state = 4;
				can->fwd(400);
				break;
			case 4: // On revient du lingot.
				state = 5;
				can->fwd(-400);
				break;
			case 5:
				state = 6;
				can->rotate(-90); // Axe bouteille.
				break;
			case 6:
				state = 7;
				can->fwd(1000); // La bouteille est pile à 1m, on y va vite.
				break;
			case 7:
				state = ATTENTE_BOUTEILLE_1;
				can->fwd(30, 30); // Avance lentement pour la toucher sans perdre en précision.
				break;
			case 8:
				state = 9;
				can->rotate(-90);
				break;
			case 9:
				state = 50;
				can->fwd(1234);
				break;
			case 50:
				state = 51;
				can->rotate(90);
				break;
			case 51:
				state = 52;
				can->fwd(20, 20);
				break;
			case 53:
				state = 54;
				can->rotate(-90);
				break;
			case 54:
				state = 10;
				can->fwd(575);
				break;
			case 10:
				state = 11;
				can->rotate(-90);
				break;
			case 11:
				state = 12;
				can->fwd(1450);
				break;
			case 12:
				state = 13;
				can->rotate(-87);
				break;
			case 13:
				state = 14;
				can->fwd(1800);
				break;
			case 14:
				state = 15;
				can->rotate(180);
				break;
			case 15:
				state = 16;
				load("recalage");
				break;
		}

		return true;
	}
};

extern "C" {
	Mission * create() {
		return new Petit();
	}
}
