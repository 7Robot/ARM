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

Can::Can(int canbus): verbose(2)
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

Task * Can::send(int id, int length, ...)
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

	return send(&packet);
}

Task * Can::send(struct libcan::can_t * packet)
{
	Task * task = new TaskCanSend(this, *packet);
	Queue::push(task);
	
	return task;
}

Task * Can::reset()
{
	if (verbose > 0) {
		printf("Can::reset()\n");
	}

	return send(127, 0);
}

Task * Can::stop()
{
	if (verbose > 0) {
		printf("Can::stop()\n");
	}

	return send(1051, 0);
}

Task * Can::rotate(int angle)
{
	if (verbose > 0) {
		printf("Can::rotate(%d)\n", angle);
	}
	
	int16_t a = round(angle*COEF_ROTATION);
	return send(1026, 2, ((char*)&a)[0], ((char*)&a)[1]);
}

Task * Can::fwd(int distance)
{
	if (verbose > 0) {
		printf("Can::fwd(%d)\n", distance);
	}

	int16_t d = round(distance*COEF_DISTANCE);
	return send(1025, 2, ((char*)&d)[0], ((char*)&d)[1]);
}

Task * Can::fwd(int left, int right, bool ramp)
{
	if (verbose > 0) {
		printf("Can::fwd(%d, %d, %s)\n", left, right, ramp?"true":"false");
	}

	return send(ramp?1032:1029, 2, left, right);
}

Task * Can::asservUp(bool up)
{
	if (verbose > 0) {
		printf("Can::up(%d)\n", up);
	}

	return send(up?1030:1031, 0);
}

Task * Can::odoSet(int16_t x, int16_t y, int16_t t)
{
	if (verbose > 0) {
		printf("Can::odoReset()\n");
	}

	return send(517, 6, ((char*)&x)[0], ((char*)&x)[1],
		((char*)&y)[0], ((char*)&y)[1],
		((char*)&t)[0], ((char*)&t)[1]);
}

Task * Can::odoReset()
{
	return odoSet(-1250, -750, 0); // FIXME mettre la bonne valeur (dépend du coté)
}

Task * Can::odoBroadcast()
{
	return send(515, 0);
}

Task * Can::odoMute()
{
	return send(514, 0);
}

Task * Can::odoRequest()
{
	return send(513, 0);
}

Task * Can::sonarRequest(int id)
{
	return send(324+id*2^5, 0);
}

Task * Can::sonarThres(int id, int16_t threshold)
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
		return send(canid, 2, ((char*)&threshold)[0], ((char*)&threshold)[1]);
	} else {
		return NULL;
	}
}

Task * Can::sonarBroadcast(int id)
{
	return send(333+id*2^5, 0);
}

Task * Can::sonarMute(int id)
{
	return send(332+id*2^5, 0);
}

void Can::recv(struct libcan::can_t packet)
{
	Task * task = new TaskCanRecv(packet);
	Queue::push(task);
}

void Can::send_now(struct can_t * packet)
{
	if (verbose > 1) {
		printf("Can::send: "); fflush(stdout);
		libcan::can_pwrite(1, dec, packet);
	}

	can_pwrite(canbus, bin, packet);
}
