#ifndef _DEMO_H_
#define _DEMO_H_

#include "Mission.h"

class Demo: public Mission
{
	public:
		Demo(CanHandler * ch);
		void run();
		void pbStateChanged(int bpID, bool state);
		void asservEnd();
		void sonarValue(int id, int edge, int value);

	private:
		int state;
		int spending;
		void consigne(int speed, int time);
		int speedconvert(int speed);

};

#endif
