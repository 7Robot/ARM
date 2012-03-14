#include <stdio.h>

#include "Mission.h"

#define BACK 163
#define LEFT 163

class Recalage: public Mission
{
	void start() {
		state = 1;
		can->rotate(90); // On se met dos au mur gauche.
	}

	bool microswitchEvent(int id, bool status) {
		switch (state) {
			case 2: // On a touché le mur gauche, on repart en avant.
				if (id == 0 && status) {
					state = 3;
					can->stop()->setDelay(700);
					can->fwd(LEFT)->setDelay(300);
				}
				break;
			case 5: // On a touché le mur de derrière, on se place enfin au centre.
				if (id == 0 && status) {
					state = 6;
					can->stop()->setDelay(700);
					can->fwd(BACK)->setDelay(300);
				}
				break;
		}
		
		return true;
	}

	bool asservDone(int erreur) {
		switch (state) {
			case 1: // Marche arrière.
				state = 2;
				can->fwd(-20, -20);
				break;
			case 3: // On se remet dos au mur normal.
				state = 4;
				can->rotate(-90);
				break;
			case 4: // On recule une dernière fos.
				state = 5;
				can->fwd(-20, -20);
				break;
			case 6:
				can->odoReset();
				//can->odoSet(-750, -1250, 9000);
				// TODO Coordonnées rouges : -750, 1250, 27000
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
