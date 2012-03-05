#ifndef _TASK_SLEEP_H_
#define _TASK_SLEEP_H_

#include "Task.h"
#include "../Mission.h"

class TaskSleep: public Task
{
	public:
		TaskSleep(int secondes, int microseconde = 0);
		void exec();

	private:
		int m_us;
		int m_s;
};

#endif
