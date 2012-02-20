#include <iostream>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "Robot.h"
#include "CallbackHandler.h"
#include "Demo.h"

using namespace std;
using namespace libcan;

Robot::Robot(int canbus)
{
	cout << "Robot::Robot()" << endl;

	this->canbus = canbus;

	this->ch = new CanHandler(canbus);
}

void Robot::start()
{
	cout << "Robot::start()" << endl;
	
	Demo demo(ch);
	cbh.push(demo.getCallbackSet());
	demo.run();
	cbh.pop();

	cout << "Robot::start() ended" << endl;
}

void Robot::recv(can_t packet)
{
	cout << "Robot::recv(" << packet.id << ")" << endl;

	if ((packet.id & 0x140) == 0x100) { // push button
		int id = ((packet.id & 0x2)>>1) + 1;
		bool state = ((packet.id & 0x1) == 1);
		(robot->cbh).pushButton(id, state);
	} else if (packet.id == 1028) { // fin commande asserv
		(robot->cbh).asserv();
	} else if ((packet.id & 0x142) == 0x142) { // sonar, edge only
		int id = ((packet.id & 0x20)>>5) + 1;
		int value = packet.b[0] + packet.b[1]*256;
		int edge =  (packet.id & 0x1) == 1;
		(robot->cbh).sonar(id, edge, value);
	} else {
		cout << "unknow packet (" << packet.id << ")" << endl;
	}
}
