#include <stdio.h>

#include "Mission.h"

void Mission::setup(MissionHandler * mh, Can * can)
{
	this->mh = mh;
	this->can = can;
}
