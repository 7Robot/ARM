#ifndef _TASK_CAN_RECV_H_
#define _TASK_CAN_RECV_H_

#include "Task.h"
#include "../Can.h"
#include "libcan.h"

using namespace libcan;

class TaskCanRecv: public Task
{
	public:
		TaskCanRecv(can_t packet);
		void exec();

	private:
		can_t m_packet;
};

#endif
