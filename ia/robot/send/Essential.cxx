#include "Send.h"

#include <math.h>

#define COEF_ROTATION 23.4
#define COEF_DISTANCE 13.4

namespace asserv {

	can_packet stop() {
		return { 1051, 0 };
	}

	can_packet rotate(int angle) {
		int16_t a = round(angle*COEF_ROTATION);
		return { 1026, 2, { ((char*)&a)[0], ((char*)&a)[1] }};
	}

	can_packet fwd(int distance) {
		int16_t d = round(distance*COEF_DISTANCE);
		return { 1025, 2, { ((char*)&d)[0], ((char*)&d)[1] }};
	}

	can_packet fwd(int left, int right, bool ramp) {
		return { ramp?1032:1029, 2, { (uint8_t)left, (uint8_t)right }};
	}
}
