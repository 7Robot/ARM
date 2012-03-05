#ifndef _TASK_LOAD_H_
#define _TASK_LOAD_H_

#include "Task.h"
#include "../Mission.h"

class TaskLoad: public Task
{
	public:
		TaskLoad(const char * mission, Mission * owner);
		void exec();

	private:
		char * m_mission;
		Mission * m_owner;
};

#endif
