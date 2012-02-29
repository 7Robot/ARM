/** Template de mission
 * Pensez à rajouter une entrée « add_library » dans le « CMakeLists.txt ».
 * Pensez également à changer le nom de votre classe en bas de ce fichier.
 */

#include <stdio.h>

#include "Mission.h"

class Template: public Mission
{
	void run() {
		printf("Template::run\n");
		load("init"); // Chargement mission « init »
		//usleep(50 * 1000); /* attente 50 ms */
		//can->fwd(500); /* avance de 500 mm */
		//can->rotate(50); /* tourne de 50° */
		//can->fwd(10, 10); /* vitesse roue gauche / roue droite (de -80 à 80) */
		wait(); /* attente signal() */
		// fin mission
	}
	
	bool microswitch(int id, bool state) {
		printf("Template::microswitch: id = %d, state = %d\n", id, state);
		if (id = 1 && !state) { // bouton avant relaché
			signal(); // on stop le wait()
		}
	}
	
	bool asserv(int erreur) {
		printf("Template::asserv: erreur = %d\n", erreur);
	}
	
	bool sonar(int id, bool edge, int value) {
		printf("Template::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}

	bool odo(int x, int y, int theta) {
		printf("Template::odo: x = %d, y = %d, theta = %d\n", x, y, theta);
	}
};

extern "C" {
	Mission * create() {
		return new Template();
	}
}

