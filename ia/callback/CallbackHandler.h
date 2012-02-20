#ifndef _CALLBACK_HANDLER_H_
#define _CALLBACK_HANDLER_H_

#include <stack>
#include "CallbackSet.h"


class CallbackHandler
{
	public:

		CallbackHandler();
		
		void push(CallbackSet cbs);
		CallbackSet apply(CallbackSet cbs);
		void pop();

		void pushButton(int bpID, bool state);
		void asserv();
		void sonar(int id, int edge, int value);

	private:

		CallbackSet pendingCbs;
		std::stack<CallbackSet> pile;
};

#endif
