#include "TaskCmp.h"

#include <stdio.h>

bool TaskCmp::operator()(Task const * t1, Task const * t2)
{
	if (t1->getPriority() < t2->getPriority()) {
		return true;
	} else {
		return t1->getId() > t2->getId();
	}
}
