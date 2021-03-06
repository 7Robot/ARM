#ifndef _TASK_UNLOAD_H_
#define _TASK_UNLOAD_H_

#include "Task.h"

class Mission;

class TaskUnload: public Task
{
	public:
		TaskUnload(Mission * mission, Mission * applicant);
		void exec();

	private:
		Mission * m_mission;
		Mission * m_applicant;
};

#endif
