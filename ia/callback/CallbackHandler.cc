#include <iostream>

#include "CallbackHandler.h"
#include "pushButtonCallback.h"
#include "sonarCallback.h"

using namespace std;

CallbackHandler::CallbackHandler()
{
	cout << "CallbackHandler::CallbackHandler()" << endl;
}

void CallbackHandler::push(CallbackSet cbs)
{
	cout << "CallbackHandler::push()" << endl;
	CallbackSet backup = apply(cbs);
	pile.push(backup);
}

CallbackSet CallbackHandler::apply(CallbackSet cbs)
{
	cout << "CallbackHandler::apply()" << endl;
	CallbackSet backup;
	if (cbs.pushButton != NULL) {
		backup.pushButton = pendingCbs.pushButton;
		pendingCbs.pushButton = cbs.pushButton;
	}
	if (cbs.asserv != NULL) {
		backup.asserv = pendingCbs.asserv;
		pendingCbs.asserv = cbs.asserv;
	}
	if (cbs.sonar != NULL) {
		backup.sonar = pendingCbs.sonar;
		pendingCbs.sonar = cbs.sonar;
	}
	return backup;
}

void CallbackHandler::pop()
{
	cout << "CallbackHandler::pop()" << endl;
	if (pile.empty()) {
		cerr << "Error: Pile vide !" << endl;
		return;
	}
	CallbackSet cbs = pile.top();
	apply(cbs);
	pile.pop();
}

void CallbackHandler::pushButton(int id, bool state)
{
	if (pendingCbs.pushButton == NULL) {
		cout << "Warning: appel d'une fonction de callback non écouté (push button)" << endl;
		cout << "ID: " << id << ", State: " << state << endl;
	} else {
		pushButtonAnswer args = { id, state };
		(pendingCbs.pushButton->call)((void*)&args);
	}
}

void CallbackHandler::asserv()
{
	if (pendingCbs.asserv == NULL) {
		cout << "Warning: appel d'une fonction de callback non écouté (asserv)" << endl;
	} else {
		(pendingCbs.asserv->call)(NULL);
	}
}

void CallbackHandler::sonar(int id, int edge, int value)
{
	if (pendingCbs.sonar == NULL) {
		cout << "Warning: appel d'une fonction de callback non écouté (sonar)" << endl;
		cout << "ID: " << id << ", Value : \t" << value << "(edge = " << edge << ")" << endl;
	} else {
		sonarAnswer args = { id, edge, value };
		(pendingCbs.sonar->call)((void*)&args);
	}
}
