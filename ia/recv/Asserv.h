#ifndef _ASSERV_H_
#define _ASSERV_H_

#include <algorithm>
#include <tuple>

#include "Can.h"

namespace recv {
    namespace asserv {
		can_callback done(std::function<void (int)> callback);
	}
}

#endif
