#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include "Demo.h"
#include "Robot.h"

#include "pushButtonCallback.h"
#include "asservCallback.h"
#include "sonarCallback.h"

using namespace std;
using namespace libcan;

Demo::Demo(CanHandler * ch): Mission(ch)
{
	cout << "Demo::Demo()" << endl;

	void (Demo::*pb)(int, bool) = &Demo::pbStateChanged;
	cbs.pushButton = new pushButtonCallback<Demo>(this, pb);

	void (Demo::*asserv)() = &Demo::asservEnd;
	cbs.asserv = new asservCallback<Demo>(this, asserv);

	void (Demo::*sonar)(int, int, int) = &Demo::sonarValue;
	cbs.sonar = new sonarCallback<Demo>(this, sonar);
}

void Demo::run()
{
	cout << "Demo::run()" << endl;

	state = 1;
		printf("state=%d\n", state);
	//consigne(20, 1000);
	ch->send(1025, 2, 100, 0);

	pause();
}

void Demo::pbStateChanged(int id, bool status)
{
	cout << "Demo: Push button, id=" << id << ", status=" << status << endl;
	
	if (id == 1 && state == 2) {
		state = 1;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
		ch->send(1025, 2, 100, 0);
	} else if (id == 2 && state == 1) {
		state = 2;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
	        ch->send(1025, 2, 145, 0);
	}
}

void Demo::asservEnd()
{
	cout << "Demo::asservEnd() (state=" << state << ")" << endl;
	
	if (state == 1) {
		state = 2;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
        	ch->send(1025, 2, 145, 0);
	} else if (state == 2) {
		state = 1;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
		ch->send(1025, 2, 100, 0);
	}
}

void Demo::sonarValue(int id, int edge, int value)
{
	printf("Demo::sonarValue(%d, %d, %d)\n", id, edge, value);

	if (state == 1 && id == 2 && edge == 1) {
		state = 2;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
        ch->send(1025, 2, 145, 0);
	} else if (state == 2 && id == 1 && edge == 1) {
		state = 1;
		printf("state=%d\n", state);
		ch->send(1029, 2, 0, 0);
		usleep(20000);
		ch->send(1025, 2, 100, 0);
	}
}
