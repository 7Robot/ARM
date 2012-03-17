#ifndef _SEND_H_
#define _SEND_H_

#include "Can.h"

namespace asserv {
	can_packet stop();
	can_packet rotate(int angle);
	can_packet fwd(int distance);
	can_packet fwd(int left, int right, bool ramp = true);
}

#endif
