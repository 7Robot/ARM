#ifndef _TASK_CAN_SEND_H_
#define _TASK_CAN_SEND_H_

#include "Task.h"
#include "../Can.h"
#include "libcan.h"

using namespace libcan;

class TaskCanSend: public Task
{
	public:
		TaskCanSend(Can * can, can_t packet);
		void exec();

	private:
		can_t m_packet;
		Can * m_can;
		int m_delai;
};

#endif
