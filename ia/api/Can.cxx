#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>

#include "Can.h"
#include "Queue.h"
#include "TaskCanSend.h"
#include "TaskCanRecv.h"

#define COEF_ROTATION 23.4
#define COEF_DISTANCE 13.4

using namespace libcan;

Can::Can(int canbus): verbose(1)
{
	printf("Can::can(%d)\n", canbus);

	this->canbus = canbus;

    can_ctx * ctx;
    if (can_init(&ctx) < 0) {
        perror("can_init");
    }

    if (can_register_callback(ctx, 0, 0, Can::recv) < 0) {
        perror("can_register_callback");
    }
    if (can_listen_on(ctx, canbus, libcan::bin) < 0) {
        perror("can_listen_on");
    }
}

bool Can::send(int id, int length, ...)
{
	int i;
	va_list ap;
	can_t packet;

	packet.id = id;
	packet.length = length;

    va_start(ap, length);
	for (i = 0 ; i < length ; i++) {
		packet.b[i] = (uint8_t)va_arg(ap, int);
	}
    va_end(ap);

	send(&packet);

	return false;
}

bool Can::send(struct libcan::can_t * packet)
{
	Queue::push(new TaskCanSend(this, *packet));
	
	return false;
}

bool Can::stop()
{
	if (verbose > 0) {
		printf("Can::stop()\n");
	}

	send(1051, 0);

	return false;
}

bool Can::rotate(int angle)
{
	if (verbose > 0) {
		printf("Can::rotate(%d)\n", angle);
	}
	
	int16_t a = round(angle*COEF_ROTATION);
	send(1026, 2, ((char*)&a)[0], ((char*)&a)[1]);
			
	/*int tics = abs(angle) * 4212 / 180;
	int low = tics % 256;
	int high = (tics - low) / 256;
	if (angle < 0)
		send(1026, 2, -high, low);
	else
		send(1026, 2, high, low);*/

	return false;
}

bool Can::fwd(int distance)
{
	if (verbose > 0) {
		printf("Can::fwd(%d)\n", distance);
	}

	int16_t d = round(distance*COEF_DISTANCE);
	send(1025, 2, ((char*)&d)[0], ((char*)&d)[1]);

	/*int tics = distance * 13.4;
	int low = tics % 256;
	int high = (tics - low) / 256;
	send(1025, 2, high, low);*/

	return false;
}

bool Can::fwd(int left, int right)
{
	if (verbose > 0) {
		printf("Can::fwd(%d, %d)\n", left, right);
	}

	send(1032, 2, left, right);

	return false;
}

bool Can::speed(int left, int right)
{
	if (verbose > 0) {
		printf("Can::speed(%d, %d)\n", left, right);
	}

	send(1029, 2, left, right);

	return false;
}

bool Can::odoSet(int16_t x, int16_t y, int16_t t) // FIXME mettre des unités mm et ° ?
{
	if (verbose > 0) {
		printf("Can::odoReset()\n");
	}

	send(517, 6, ((char*)&x)[0], ((char*)&x)[1],
		((char*)&y)[0], ((char*)&y)[1],
		((char*)&t)[0], ((char*)&t)[1]);

	return false;
}

bool Can::sonarThres(int id, int16_t threshold)
{
	if (verbose > 0) {
		printf("Can::sonarThres(%d, %d)\n", id, threshold);
	}

	int canid = 0;
	if (id == 0) {
		canid = 328;
	} else if (id == 1) {
		canid = 360;
	}
	if (canid != 0) {
		send(canid, 2, ((char*)&threshold)[0], ((char*)&threshold)[1]);
	}

	return false;
}

void Can::recv(struct libcan::can_t packet)
{
	/*if (verbose > 1) {
		printf("Can::recv: "); fflush(stdout);
		libcan::can_pwrite(1, libcan::dec, &packet);
	}*/

	Queue::push(new TaskCanRecv(packet));
}

bool Can::send_now(struct can_t * packet)
{
	if (verbose > 1) {
		printf("Can::send: "); fflush(stdout);
		libcan::can_pwrite(1, dec, packet);
	}

	can_pwrite(canbus, bin, packet);

	return false;
}
