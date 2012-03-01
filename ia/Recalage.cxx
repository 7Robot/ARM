#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

#define PROFONDEUR_RECALAGE        87 // mm
#define DISTANCE_RECALAGE_DEPART   (250 - PROFONDEUR_RECALAGE) // (=163) mm

class Recalage: public Mission
{
	void run() {
		printf("Recalage::run\n");
		name = "Recalage";
		state = 1;
		can->rotate(90); // On se met dos au mur gauche.
	}

	bool microswitch(int id, bool status) {
		switch (state) {
			case 2: // On a touché le mur gauche, on repart en avant.
				if (id == 0 && status) {
					usleep(700000);
					can->stop();
					state = 3;
					usleep(300000);
					can->fwd(DISTANCE_RECALAGE_DEPART);
				}
				break;
			case 5: // On a touché le mur de derrière, on se place enfin au centre.
				if (id == 0 && status) {
					usleep(700000);
					can->stop();
					state = 6;
					usleep(300000);
					can->fwd(DISTANCE_RECALAGE_DEPART);
				}
				break;
		}
		
		return true;
	}

	bool asserv(int erreur) {
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
