#include "TaskCanRecv.h"

#include "../Spread.h"

#include "libcan.h"

using namespace std;

TaskCanRecv::TaskCanRecv(can_t packet): m_packet(packet) {}

void TaskCanRecv::exec()
{
	Spread::packet(m_packet);
}
