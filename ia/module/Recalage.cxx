#include "Module.h"

#include "recv/Asserv.h"

#include <stdio.h>

class Recalage: public Module
{	
	void init() {
		//can->bind(recv::asserv::done([this](int error){this->asservDone(error);}));
		bind(recv::asserv::done(std::bind(&Recalage::asservDone,this, std::placeholders::_1)));
	}

	void asservDone(int error) {
		printf("asservDone %d\n", error);
	}

};

extern "C" {
	Module * create() {
		return new Recalage();
	}
}
