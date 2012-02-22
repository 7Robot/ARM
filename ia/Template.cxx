/** Template de mission
 * Pensez à rajouter une entrée « add_library » dans le « CMakeLists.txt ».
 * Pensez également à changer le nom de votre classe en bas de ce fichier.
 */

#include <stdio.h>
#include <unistd.h>

#include "Mission.h"
#include "Callback.h"

class Template: public Mission
{
	public:
	
	int state;

	private:

	void init() {
		printf("Demo::init()\n");

		this->cb_microswitch = new CB_Microswitch<Template>(this, &Template::microswitch);
		this->cb_asserv = new CB_Asserv<Template>(this, &Template::asserv);
		this->cb_sonar = new CB_Sonar<Template>(this, &Template::sonar);
	}
	
	void run() {
		printf("Demo::run\n");
		can->forward(0); /* initialisation dû a un bug de Maxime, tapez-le */
		usleep(50 * 1000); /* 50 ms */
		can->forward(50); /* distance en mm */
		usleep(1000 * 1000);
		can->rotate(50); /* angle en degré */
		usleep(1000 * 1000); /* 1 sec */
		can->forward(10, 10); /* vitesse roue gauche, roue droite */
		pause();
	}
	
	void microswitch(int id, bool state) {
		printf("Demo::microswitch: id = %d, state = %d\n", id, state);
	}
	
	void asserv(int erreur) {
		printf("Demo::asserv: erreur = %d\n", erreur);
	}
	
	void sonar(int id, bool edge, int value) {
		printf("Template::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);
	}
};

extern "C" {
	Mission * create() {
		return new Template();
	}
}

