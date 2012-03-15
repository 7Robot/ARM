#ifndef _CAN_H_
#define _CAN_H_

#include "Task.h"
#include "Queue.h"

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
		int bind(can_callback callback);
		void unbind(int bindid);

	private:
		std::thread m_thr;
		int m_canbus;
		int m_bindid;
		Queue * m_queue;
		std::map<int, can_callback> cbs;

		void send(can_packet);
		void recv();
		void dispatch(can_packet);
};

#endif
