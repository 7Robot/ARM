#ifndef _CAN_H_
#define _CAN_H_

#include "queue/Queue.h"

#include <stdint.h>

#include <thread>
#include <tuple>
#include <algorithm>
#include <map>

typedef struct {
	int id;
	int length;
	uint8_t b[8];
} can_packet;

typedef std::tuple<int, int, std::function<void (can_packet)>> can_callback;

class Can
{
	public:
		Can(int canbus, Queue * queue = NULL);
		void lisen();
		void wait();
		Task * push(can_packet);
		Task * push(int id, int length, ...);
		void bind(void*, can_callback callback);
		void unbind(void* id);

	private:
		std::thread m_thr;
		int m_canbus;
		Queue * m_queue;
		std::multimap<void*, can_callback> cbs;

		void send(can_packet);
		void recv();
		void dispatch(can_packet);
};

#endif
