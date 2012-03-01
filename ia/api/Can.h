#ifndef _CAN_H_
#define _CAN_H_

#include "libcan.h"

class Can
{
	public:
		Can(int canbus, void (*)(struct libcan::can_t));
		bool send(int id, int length, ...);
		bool send(struct libcan::can_t * packet);
		bool stop();
		bool rotate(int angle); /* degré */
		bool fwd(int distance); /* mm */
		bool fwd(int left_speed, int right_speed); /* -80 à 80 */
		bool speed(int left_speed, int right_speed); /* -80 à 80 */
		bool odoReset();
		bool sonarThres(int id, int threshold);

	private:
		int canbus;
	
};

#endif
