#include <iostream>

#include "Mission.h"

using namespace std;

Mission::Mission(CanHandler * ch)
{
	cout << "Mission::Mission()" << endl;

	this->ch = ch;
}

CallbackSet Mission::getCallbackSet()
{
	return cbs;
}
