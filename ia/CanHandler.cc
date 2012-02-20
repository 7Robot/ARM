#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "CanHandler.h"
#include "Robot.h"

using namespace std;
using namespace libcan;

CanHandler::CanHandler(int canbus)
{
	cout << "CanHandler::CanHandler()" << endl;

	this->canbus = canbus;

    can_ctx * ctx;
    if (can_init(&ctx) < 0) {
        perror("can_init");
        exit(1);
    }

	printf("register…");
    if (can_register_callback(ctx, 0, 0, Robot::recv) < 0) {
        perror("can_register_callback");
        exit(1);
    }
	printf("ok\n");
    if (can_listen_on(ctx, canbus, libcan::bin) < 0) {
        perror("can_listen_on");
        exit(1);
    }
}

bool CanHandler::send(int id, int length, ...)
{
	cout << "CanHendler::send(" << id << ")" << endl;
	
	va_list ap;

    va_start(ap, length);
	can_vwrite(canbus, bin, id, length, ap);
    va_end(ap);

	return true;
}

bool CanHandler::send(struct can_t * packet)
{
	can_pwrite(canbus, bin, packet);
}
