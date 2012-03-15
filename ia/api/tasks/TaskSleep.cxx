#include "TaskSleep.h"

#include <unistd.h>

using namespace std;

TaskSleep::TaskSleep(int s, int us): m_s(s), m_us(us) {}

void TaskSleep::exec()
{
	if (m_s)
		sleep(m_s);
	if (m_us)
		usleep(m_us);
}
