#ifndef _CALLBACK_SET_H_
#define _CALLBACK_SET_H_

#include <iostream>

class Callback
{
	public:
		virtual void call(void * args)=0;
};

class CallbackSet
{
	public:

		CallbackSet();

		Callback * pushButton;
		Callback * asserv;
		Callback * sonar;
};

#endif
