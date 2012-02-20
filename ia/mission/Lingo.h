#ifndef _LINGO_H_
#define _LINGO_H_

#include "Mission.h"

class Lingo: public Mission
{
	public:
		Lingo(CanHandler * ch);
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
