#ifndef _RECV_H_
#define _RECV_H_

#include <algorithm>
#include <tuple>

#include "Can.h"

namespace recv {
    namespace asserv {
		can_callback done(std::function<void (int)>);
	}
	namespace captor {
		can_callback microswitch(std::function<void (int, bool)>);
	}
}

#endif
