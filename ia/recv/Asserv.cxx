#include "Asserv.h"
#include "Can.h"

#include <algorithm>

namespace recv {
	
	namespace asserv {

		std::tuple<int, int, std::function<void (can_packet)>> done(std::function<void (int)> callback) {
			return std::make_tuple(125, 23, [callback](can_packet packet) {
				int error = packet.b[0];
				callback(error);
			});
		}

	}

}
