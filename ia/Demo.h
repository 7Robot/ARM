
#include "Mission.h"

class Demo: public Mission
{
	public:
		void init();
		void run();
		void cb_asserv(int erreur);
};
