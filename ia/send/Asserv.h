#ifndef _ASSERV_H_
#define _ASSERV_H_

#include "Can.h"

namespace asserv {
	can_packet stop();
	can_packet rotate(int angle);
}

#endif
