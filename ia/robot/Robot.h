#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "queue/Queue.h"
#include "Can.h"
#include "ModuleManager.h"

class Robot
{
	public:
		Robot(int canbus, const char * module);
		Module * load(const char * module);

	private:
		Queue m_queue;
		Can m_can;
		ModuleManager m_mh;
};

#endif
