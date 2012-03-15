#include "Asserv.h"
#include "Can.h"

namespace recv {
	
	namespace asserv {

		can_callback done(std::function<void (int)> callback) {
			return  std::make_tuple(2047, 1028, [callback](can_packet packet) {
				int error = packet.b[0];
				callback(error);
			});
		}

	}

}
