#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include "Lingo.h"
#include "Robot.h"

#include "pushButtonCallback.h"
#include "asservCallback.h"
#include "sonarCallback.h"

using namespace std;
using namespace libcan;

Lingo::Lingo(CanHandler * ch): Mission(ch)
{
	cout << "Lingo::Lingo()" << endl;

	void (Lingo::*pb)(int, bool) = &Lingo::pbStateChanged;
	cbs.pushButton = new pushButtonCallback<Lingo>(this, pb);

	void (Lingo::*asserv)() = &Lingo::asservEnd;
	cbs.asserv = new asservCallback<Lingo>(this, asserv);

	void (Lingo::*sonar)(int, int, int) = &Lingo::sonarValue;
	cbs.sonar = new sonarCallback<Lingo>(this, sonar);
}

void Lingo::run()
{
	cout << "Demo::run()" << endl;

	state = 1;
	printf("state=%d\n", state);
	ch->send(1025, 2, 20, 0);

	pause();
}

void Lingo::pbStateChanged(int id, bool status)
{
	cout << "Demo: Push button, id=" << id << ", status=" << status << endl;
	
	/*if (id == 1 && state == 2) {
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
	}*/
	//ch->send(1029, 2, 0, 0);
}

void Lingo::asservEnd()
{
	cout << "Demo::asservEnd() (state=" << state << ")" << endl;
	
	if (state == 1) {
		state = 2;
		printf("state=%d\n", state);
		ch->send(1026, 2, 8, 58);
		//usleep(20000);
        	//ch->send(1025, 2, 145, 0);
	} else if (state == 2) {
		state = 3;
		printf("state=%d\n", state);
		ch->send(1025, 2, 60, 0);
		//usleep(20000);
		//ch->send(1025, 2, 100, 0);
	} else if (state == 3) {
		state = 4;
		printf("state=%d\n", state);
		ch->send(1026, 2, 8, 58);
	} else if (state == 4) {
		state = 5;
		printf("state=%d\n", state);
		ch->send(1025, 2, 30, 0);
	} else if (state == 5) {
		state = 6;
		printf("state=%d\n", state);
		ch->send(1025, 2, 225, 0);	
	}
}

void Lingo::sonarValue(int id, int edge, int value)
{
	printf("Demo::sonarValue(%d, %d, %d)\n", id, edge, value);

	/*if (state == 1 && id == 2 && edge == 1) {
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
	}*/
}
