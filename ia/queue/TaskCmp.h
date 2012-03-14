#ifndef _TASK_CMP_H_
#define _TASK_CMP_H_

#include "TaskBase.h"

class TaskCmp
{
	public:
		bool operator()(TaskBase const * t1, TaskBase const * t2);
};

#endif
