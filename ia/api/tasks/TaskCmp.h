#ifndef _TASK_CMP_H_
#define _TASK_CMP_H_

#include "Task.h"

class TaskCmp
{
	public:
		bool operator()(Task const * t1, Task const * t2);
};

#endif
