/** Template de mission
 * Pensez à rajouter une entrée « add_library » dans le « CMakeLists.txt ».
 * Pensez également à changer le nom de votre classe en bas de ce fichier.
 */

#include <stdio.h>
#include <unistd.h>

#include "Mission.h"

class Template: public Mission
{
	void init() {
		printf("Template::init()\n");
	}
	
	void run() {
		printf("Template::run\n");
		can->fwd(0); /* initialisation dû a un bug de Maxime, tapez-le */
		usleep(50 * 1000); /* 50 ms */
		can->fwd(500); /* distance en mm */
		//usleep(1000 * 1000);
		//can->rotate(50); /* angle en degré */
		//usleep(1000 * 1000); /* 1 sec */
		//can->fwd(10, 10); /* vitesse roue gauche, roue droite */
		pause();
	}
	
	void microswitch(int id, bool state) {
		printf("Template::microswitch: id = %d, state = %d\n", id, state);
	}
	
	void asserv(int erreur) {
		printf("Template::asserv: erreur = %d\n", erreur);
	}
	
	void sonar(int id, bool edge, int value) {
		printf("Template::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}

	void odo(int x, int y, int theta) {
		printf("Template::odo: x = %d, y = %d, theta = %d\n", x, y, theta);
	}
};

extern "C" {
	Mission * create() {
		return new Template();
	}
}

