#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "Can.h"

using namespace libcan;

bool Can::setup(int canbus, void (*callback)(struct can_t))
{
	//printf("Can::setup(%d, %p)\n", canbus, callback);

	this->canbus = canbus;

    can_ctx * ctx;
    if (can_init(&ctx) < 0) {
        perror("can_init");
        return true;
    }

    if (can_register_callback(ctx, 0, 0, callback) < 0) {
        perror("can_register_callback");
        return true;
    }
    if (can_listen_on(ctx, canbus, libcan::bin) < 0) {
        perror("can_listen_on");
        return true;
    }

	return false;
}

bool Can::send(int id, int length, ...)
{
	//printf("Can::send(%d, %d, ...)\n", id, length);
	va_list ap;

    va_start(ap, length);
	can_vwrite(canbus, bin, id, length, ap);
    va_end(ap);

	return false;
}

bool Can::send(struct libcan::can_t * packet)
{
	//printf("Can::send(packet)\n");
	
	can_pwrite(canbus, bin, packet);

	return false;
}

bool Can::rotate(int angle)
{
	printf("Can::rotate(%d)\n", angle);
	
	int tics = abs(angle) * 4212 / 180;
	int low = tics % 256;
	int high = (tics - low) / 256;
	if (angle < 0)
		send(1026, 2, -high, low);
	else
		send(1026, 2, high, low);

	return false;
}

bool Can::fwd(int distance)
{
	printf("Can::fwd(%d)\n", distance);

	int tics = distance * 13.4;
	int low = tics % 256;
	int high = (tics - low) / 256;
	send(1025, 2, high, low);

	return false;
}

bool Can::fwd(int left, int right)
{
	printf("Can::fwd(%d, %d)\n", left, right);

	send(1032, 2, left, right);

	return false;
}

bool Can::speed(int left, int right)
{
	printf("Can::speed(%d, %d)\n", left, right);

	send(1029, 2, left, right);

	return false;
}
