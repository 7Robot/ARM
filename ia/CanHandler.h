#ifndef _CAN_HANDLER_H_
#define _CAN_HANDLER_H_

#include "libcan.h"

class CanHandler
{
	public:
		CanHandler(int canbus);
		bool send(int id, int length, ...);
		bool send(struct libcan::can_t * packet);

	private:
		int canbus;
	
};

#endif
