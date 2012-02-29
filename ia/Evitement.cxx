/** Template de mission
 * Pensez à rajouter une entrée « add_library » dans le « CMakeLists.txt ».
 * Pensez également à changer le nom de votre classe en bas de ce fichier.
 */

#include <stdio.h>

#include "Mission.h"

#define SEUIL 6
#define UP	10
#define DOWN	-40

class Evitement: public Mission
{
	void run() {
		printf("Mission::run\n");
		can->sonarThres(0, SEUIL * 256);
		usleep(50 * 1000); /* attente 50 ms */
		can->sonarThres(1, SEUIL * 256);
		usleep(50 * 1000); /* attente 50 ms */
		//can->fwd(500); /* avance de 500 mm */
		//can->rotate(50); /* tourne de 50° */
		can->fwd(50, 50); /* vitesse roue gauche / roue droite (de -80 à 80) */
		state = 1;
		//wait(); /* attente signal() */
		// fin mission
	}
	
	/*void microswitch(int id, bool state) {
		printf("Template::microswitch: id = %d, state = %d\n", id, state);
		if (id = 1 && !state) { // bouton avant relaché
			signal(); // on stop le wait()
		}
	}*/
	
	/*void asserv(int erreur) {
		printf("Template::asserv: erreur = %d\n", erreur);
	}*/
	
	bool sonar(int id, bool edge, bool nearby, int value) {
		printf("Evitement::sonar: id = %d, edge = %d, nearby = %d, value = %d\n", id, edge, nearby, value);
		switch (state) {
			case 1:
				if (id == 0 && nearby) {
					state = 2;
					can->fwd(15, -40);
				} else if (id == 1 && nearby) {
					state = 3;
					can->fwd(-40, 15);
				}
				break;
			case 2:
				if (id == 0 && !nearby) {
					state = 1;
					can->fwd(50, 50);
				}
				break;
			case 3:
				if (id == 1 && !nearby) {
					state = 1;
					can->fwd(50, 50);
				}
				break;
		}
	}

	/*void odo(int x, int y, int theta) {
		printf("Template::odo: x = %d, y = %d, theta = %d\n", x, y, theta);
	}*/
};

extern "C" {
	Mission * create() {
		return new Evitement();
	}
}

