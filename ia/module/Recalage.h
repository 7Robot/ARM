#ifndef _RECALAGE_H_
#define _RECALAGE_H_

#include "Can.h"

class Recalage//: public Module
{
	public:
		Recalage(Can * can);

	private:
		void asservDone(int error);
};

#endif
