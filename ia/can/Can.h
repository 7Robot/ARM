#ifndef _CAN_H_
#define _CAN_H_

#include "Task.h"

#include <stdint.h>

#include <thread>
#include <tuple>
#include <algorithm>

typedef struct {
	int id;
	int length;
	uint8_t b[8];
} can_packet;

class Can
{
	public:
		Can(int canbus);
		void lisen();
		void wait();
		Task * push(can_packet);
		Task * push(int id, int length, ...);
		void bind(std::tuple<int, int, std::function<void (can_packet)>> callback);

	private:
		std::thread * thr;
		int m_canbus;
		std::vector<std::tuple<int, int, std::function<void (can_packet)>>> cbs;

		void send(can_packet);
		void recv();
		void dispatch(can_packet);
};

#endif
