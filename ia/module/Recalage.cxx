#include "Recalage.h"

#include "recv/Asserv.h"

#include <stdio.h>
#include <tuple>

Recalage::Recalage(Can * can)
{
	can->bind(recv::asserv::done([this](int error){this->asservDone(error);}));
}

void Recalage::asservDone(int error) {
	printf("asservDone %d\n", error);
}
