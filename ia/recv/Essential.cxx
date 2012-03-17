#include "Recv.h"

namespace recv {
	namespace asserv {
		can_callback done(std::function<void (int)> cb) {
			return  std::make_tuple(2047, 1028, [cb](can_packet p) {
				int error = 0;
				if (p.length == 1) {
					int error = p.b[0];
				}
				cb(error);
			});
		}
	}
	namespace captor {
		can_callback microswitch(std::function<void (int, bool)> cb) {
			return std::make_tuple(2044, 256, [cb](can_packet p) {
				int id = (p.id & 2) >> 1;
				bool status = (p.id & 1 == 1);
				cb(id, status);
			});
		}
	}
}
