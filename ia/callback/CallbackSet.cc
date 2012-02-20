#include <iostream>

#include "CallbackSet.h"

using namespace std;

CallbackSet::CallbackSet()
{
	cout << "CallbackSet::CallbackSet()" << endl;

	pushButton = NULL;
	asserv = NULL;
	sonar = NULL;

	/*sharpAnswer = NULL;
	sharpInterval = -1;
	
	lineCaptorAnswer = NULL;
	lineCaptorInterval = -1;
	
	pbStateChanged = NULL;*/
}
