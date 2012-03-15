#include "Asserv.h"

#include <math.h>

#define COEF_ROTATION 23.4
#define COEF_DISTANCE 13.4

namespace asserv {

	can_packet stop() {
		return { 1051, 0 };
	}

	can_packet rotate(int angle) {
		int16_t a = round(angle*COEF_ROTATION);
		return { 1026, 2, { ((char*)&a)[0], ((char*)&a)[1] } };
	}
}
