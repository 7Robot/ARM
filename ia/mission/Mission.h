#ifndef _MISSION_H_
#define _MISSION_H_

#include "../callback/CallbackSet.h"
#include "../CanHandler.h"

class Mission
{

	public:
		Mission(CanHandler * ch);
		virtual void run() = 0;
		CallbackSet getCallbackSet();

	protected:
		CallbackSet cbs;
		CanHandler * ch;

};

#endif
