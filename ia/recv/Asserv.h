#ifndef _ASSERV_H_
#define _ASSERV_H_

#include <algorithm>
#include <tuple>

#include "Can.h"

namespace recv {
    namespace asserv {
		std::tuple<int, int, std::function<void (can_packet)>> done(std::function<void (int)> callback);
	}
}

#endif
