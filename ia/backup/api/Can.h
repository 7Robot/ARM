#ifndef _CAN_H_
#define _CAN_H_

#include "libcan.h"
#include "tasks/Task.h"

class Can
{
	public:
		Can(int canbus);
		
		// These functions add a Task to the task queue :
		Task * send(int id, int length, ...);
		Task * send(struct libcan::can_t * packet);

		Task * reset(); // 127
		Task * stop(); // 1051
		Task * rotate(int angle); // 1026, degré
		Task * fwd(int distance); // 1025, mm
		Task * fwd(int left_speed, int right_speed, bool ramp = true); // 1029/1032, -80 à 80
		Task * asservUp(bool up); // 1030/1031
		Task * odoSet(int16_t x, int16_t y, int16_t t); // 517
		Task * odoReset(); // 517
		Task * odoBroadcast(); // 515
		Task * odoMute(); // 514
		Task * odoRequest(); // 513
		Task * sonarRequest(int id); // 324/356
		Task * sonarThres(int id, int16_t threshold); // 328/360
		Task * sonarBroadcast(int id); // 333/365
		Task * sonarMute(int id); // 332/364

		// These functions speak directly with the bus.
		static void recv(struct libcan::can_t packet);
		void send_now(struct libcan::can_t * packet);

	private:
		int canbus;
		int verbose;
	
};

#endif
