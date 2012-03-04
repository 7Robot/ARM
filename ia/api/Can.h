#ifndef _CAN_H_
#define _CAN_H_

#include "libcan.h"

class Can
{
	public:
		Can(int canbus);
		
		// These functions add a Task to the task queue :
		bool send(int id, int length, ...);
		bool send(struct libcan::can_t * packet);
		bool stop();
		bool rotate(int angle); /* degré */
		bool fwd(int distance); /* mm */
		bool fwd(int left_speed, int right_speed); /* -80 à 80 */
		bool speed(int left_speed, int right_speed); /* -80 à 80 */
		bool odoSet(int16_t x, int16_t y, int16_t t);
		bool sonarThres(int id, int16_t threshold);

		// These functions speak directly with the bus.
		static void recv(struct libcan::can_t packet);
		bool send_now(struct libcan::can_t * packet);

	private:
		int canbus;
		int verbose;
	
};

#endif
