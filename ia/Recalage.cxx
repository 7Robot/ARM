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
					msleep(700);
					can->stop();
					state = 3;
					msleep(300);
					can->fwd(LEFT);
				}
				break;
			case 5: // On a touché le mur de derrière, on se place enfin au centre.
				if (id == 0 && status) {
					msleep(700);
					can->stop();
					state = 6;
					msleep(300);
					can->fwd(BACK);
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
				can->odoSet(-750, -1250, 9000);
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
