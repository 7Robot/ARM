#include <stdio.h>

#include "Mission.h"
#include "Can.h"

void Mission::setup(Can * can)
{
	this->can = can;
}
