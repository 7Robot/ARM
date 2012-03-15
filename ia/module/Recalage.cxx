#include "Recalage.h"

#include "recv/Asserv.h"

#include <stdio.h>
#include <tuple>

Recalage::Recalage(Can * can)
{
	std::function<void (int)> cb = std::bind(&Recalage::asservDone, this, std::placeholders::_1);
	//std::tuple<int, int, std::function<void (can_packet)>> pcb = done(cb);
	//can->bind(pcb);
}

void Recalage::asservDone(int error) {
	printf("asservDone %d\n", error);
}
