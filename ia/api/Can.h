#ifndef _CAN_H_
#define _CAN_H_

#include "libcan.h"

class Can
{
	public:
		bool setup(int canbus, void (*)(struct libcan::can_t));
		bool send(int id, int length, ...);
		bool send(struct libcan::can_t * packet);
		bool rotate(int angle); /* degré */
		bool forward(int distance); /* mm */
		bool forward(int left_speed, int right_speed); /* -80 à 80 */

	private:
		int canbus;
	
};

#endif
