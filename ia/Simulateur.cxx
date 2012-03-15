#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "Mission.h"
#include <sys/time.h>
#include "libcan.h"

#define TIME_REFRESH 100
#define TIME_DISTANCE 8
#define TIME_ROTATION 0.06
#define ODO_MUTE false
#define COEF_ROTATION 23.4
#define COEF_DISTANCE 13.4
#define PI 3.1515927

enum asservType {
	DISTANCE,
	SPEED,
	SPEEDRAMP,
	ROTATION,
	STOP
};

static asservType asservStatus = STOP;
static bool odoMute;

static int speed_left;
static int speed_right;

static int16_t x;
static int16_t y;
static uint16_t theta;

static long int time_s;
static int16_t x_s;
static int16_t y_s;
static uint16_t theta_s;

static long int time_e;
static int16_t x_e;
static int16_t y_e;
static uint16_t theta_e;

long int getTime()
{
	static struct timeval tv;
	static struct timezone tz;
	gettimeofday(&tv, &tz);
	localtime(&tv.tv_sec);
	return (long int)tv.tv_sec * 1000000 + (long int)tv.tv_usec;
}

void setTimeStart()
{
	time_s = getTime();
}

void setTimeEnd(int duration)
{
	time_e = getTime() + (long int)duration * 1000;
}

double getProgression()
{
	long int time = getTime();
	long int elapsed = time - time_s;
	long int duration = time_e - time_s;
	double progression = (double)elapsed/(double)duration;
	//printf("Simulator::getProgression: %ld → %ld = %ld, %ld → %ld = %ld, %f\n", time_s, time_e, duration, time_s, time, elapsed, progression);
	return progression;
}

static void updatePos()
{
	double progress;
	switch (asservStatus) {
		case DISTANCE:
		case ROTATION:
			printf("Calcul dist/rot pos\n");
			progress = getProgression();
			if (progress < 1) {
				x = x_s + (x_e - x_s)*progress;
				y = y_s + (y_e - y_s)*progress;
				theta = ((int)theta_s + (int)(theta_e - theta_s)*progress + (int)36000);
				theta %= 36000;
			} else {
				x = x_e;
				y = y_e;
				theta = theta_e;
				asservStatus = STOP;
			}
			break;
		case SPEEDRAMP:
		case SPEED:
			long int time = getTime() - time_s;
			x = x_s + (speed_left * time / 400000)*cos(fabs(theta)*PI/18000.0);
			y = y_s + (speed_left * time / 400000)*sin(fabs(theta)*PI/18000.0);
			break;
	}
}

static void odoSend(Can * can)
{
	updatePos();
	libcan::can_t p;
	p.id = 516;
	p.length = 6;
	p.b[0] = ((uint8_t*)&x)[0];
	p.b[1] = ((uint8_t*)&x)[1];
	p.b[2] = ((uint8_t*)&y)[0];
	p.b[3] = ((uint8_t*)&y)[1];
	p.b[4] = ((uint8_t*)&theta)[0];
	p.b[5] = ((uint8_t*)&theta)[1];
	can->send(&p);
}

static void * odoFlood(void * arg)
{
	while (1) {
		if (!odoMute) {
			odoSend((Can*)arg);
		}
		usleep(TIME_REFRESH * 1000);
	}
}

class Simulateur: public Mission
{	
	void start() {
		x = 0; y = 0; theta = 0;
		odoMute = ODO_MUTE;
		pthread_t pth;
		asservStatus = STOP;
		pthread_create(&pth, NULL, odoFlood, can);
	}
	
	bool asservLineOrder(int distance) {
		updatePos();
		x_s = x; y_s = y; theta_s = theta;
		//printf("theta : %d, %f, %f\n", theta, (double)cos(theta), (double)sin(theta));
		x_e = x_s + (double)distance * cos(fabs(theta)*PI/18000.0);
		y_e = y_s + (double)distance * sin(fabs(theta)*PI/18000.0);
		theta_e = theta_s;
		//printf("%d %d %d\n", x_e, y_e, theta_e);
		int duration = fabs(distance)*TIME_DISTANCE;
		setTimeStart();
		setTimeEnd(duration);
		can->send(1028, 0)->setDelay(duration);
		asservStatus = DISTANCE;
		return true;
	}

	bool asservRotOrder(int angle) {
		updatePos();
		x_s = x; y_s = y; theta_s = theta;
		int duration = fabs(angle)*TIME_ROTATION;
		setTimeStart();
		setTimeEnd(duration);
		x_e = x_s;
		y_e = y_s;
		theta_e = ((int)theta_s + angle) % 36000;
		can->send(1028, 0)->setDelay(duration);
		asservStatus = ROTATION;
		return true;
	}

	bool asservSpeedOrder(int left, int right, bool withRamp) {
		updatePos();
		x_s = x; y_s = y; theta_s = theta;
		speed_left = left;
		speed_right = right;
		setTimeStart();
		asservStatus = withRamp?SPEEDRAMP:SPEED;
		return true;
	}

	bool asservStop() {
		updatePos();
		asservStatus = STOP;
		return true;
	};

	bool asservUp(bool up) {
		printf("WARNING: asservUp not implementing.\n");
		return true;
	}

	bool globalReset() {
		printf("WARNING: globalReset not implementing.\n");
		return true;
	}

	bool odoRequest() {
		odoSend(can);
		return true;
	}

	bool odoSet(int _x, int _y, int _theta) {
		updatePos();
		x_s += -x + _x;
		y_s += -y + _y;
		theta_s += -theta + _theta;
		x_e += -x + _x;
		y_e += -y + _y;
		theta_e += -theta + _theta;
		x = _x;
		y = _y;
		theta = _theta;
		return true;
	}

	bool odoBroadcast(bool broadcast) {
		odoMute = !broadcast;	
		return true;
	}
};

extern "C" {
	Mission * create() {
		return new Simulateur();
	}
}

