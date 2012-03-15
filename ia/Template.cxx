/** Template de mission
 * Pensez à rajouter une entrée « add_library » dans le « CMakeLists.txt ».
 * Pensez également à changer le nom de votre classe en bas de ce fichier.
 */

#include <stdio.h>

#include "Mission.h"

class Template: public Mission
{
	void start() {
		load("fwd"); /* chargement mission « init » */
		can->fwd(500)->setDelay(50); /* avance de 500 mm  dans 50 ms */
		can->rotate(50); /* tourne de 50° */
		can->fwd(10, 10); /* vitesse roue gauche / roue droite (de -80 à 80) */
		can->stop(); /* arrête instanément le robot */
		can->odoSet(10, 10, 90); /* set odométrie x=10, y=10, theta=90° */
		can->sonarThres(2, 1500); /* set seuil sonar 2 à 1500 */
		state = 1; /* modifie l'état de la mission */
	}

	bool missionDone(Mission * mission) { // Une mission est terminé.
		printf("Template::missionDone: %p (%s)\n", mission, mission->getName());

		return true; // Continuer la propagation de l'event aux autres mission
	}

	bool missionLoaded(Mission * mission) { // Une mission a été chargé, vérifiez que mission != NULL !
		printf("Template::missionLoaded: %p (%s)\n", mission, mission==NULL?"(null)":mission->getName());
		if (mission != NULL) {
			unload(mission); /* déchargement mission « init » */
		}

		return true; // Continuer la propagation de l'event aux autres mission.
	}
	
	bool microswitchEvent(int id, bool pressed) { // Un microswitch a été enfoncé ou relaché
		// id : id du bouton
		// pressed : bouton enfoncé
		printf("Template::microswitch: id = %d, pressed = %d\n", id, pressed);
		if (id = 1 && !pressed) { // bouton avant relaché
			end(); // Quitte la mission
		}

		return false; // Stopper la propagation de l'event aux autres mission
	}
	
	bool asservDone(int error) { // Fin consigne asservissement
		// error : erreur de l'asservissement à la fin de la consigne
		// (de l'ordre de 30 tics)
		printf("Template::asserv: erreur = %d\n", error);

		return true; // Continuer la propagation de l'event aux autres mission
	}
	
	bool sonarEvent(int id, bool edge, bool nearby, int value) { // Réception valeur sonar
		// id : id du sonar
		// edge : si il s'agit d'un front (passage en dessous ou au dessus du
		// seuil)
		// nearby : valeur sous le seuil (objet proche)
		// value : distance détecté
		printf("Template::sonar: id = %d, edge = %d, value = %d\n", id, edge, value);

		return true; // Continuer la propagation de l'event aux autres mission
	}

	bool odoEvent(int x, int y, int theta) { // Réception valeur odométrie
		printf("Template::odo: x = %d, y = %d, theta = %d\n", x, y, theta);

		return true; // Continuer la propagation de l'event aux autres mission
	}

	bool canEvent(struct libcan::can_t packet) // Réception d'un packet non décodé par l'api
	{
		printf("Template::canEvent: unknow packet\n");

		return true; // Continuer la propagation de l'event aux autres mission
	}
};

extern "C" {
	Mission * create() {
		return new Template();
	}
}

