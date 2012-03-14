#include "TaskCmp.h"

#include <stdio.h>

bool TaskCmp::operator()(TaskBase const * t1, TaskBase const * t2)
{
	if (t1->getPriority() < t2->getPriority()) {
		return true;
	} else {
		return t1->getId() > t2->getId();
	}
}
