#include "TaskCanSend.h"
#include "libcan.h"
#include <stdio.h>

using namespace std;

TaskCanSend::TaskCanSend(Can * can, can_t packet):
	m_can(can), m_packet(packet) {}

void TaskCanSend::exec()
{
	m_can->send_now(&m_packet);
}
