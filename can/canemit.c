#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#include "libcan.h"

#define DELAY 1000000
#define COUNT -1

typedef struct can_t can_t;

int getargs(int argc, char * argv[], int * delay, int * count);

void help(char const * cmd)
{
	printf("Usage: %s [-d delay(ms)] [-c count] ID B1 B2 ...\n", cmd);
}

int main (int argc, char* argv[])
{
	int delay = DELAY;
	int count = COUNT;
	can_t packet;

	int pos = getargs(argc, argv, &delay, &count);

	if (argc - pos == 0) {

		struct timeval tv;
		struct timezone tz;
		struct tm * tm;
		gettimeofday(&tv, &tz);
		tm = localtime(&tv.tv_sec);
		srand(((tm->tm_hour*60+tm->tm_min)*60+tm->tm_sec)*1000000+tv.tv_usec);

		while (count) {
			if (count > 0)
				count--;
			packet.id = rand() % 2048;
			packet.length = rand() % 9;
			for (int i = 0 ; i < packet.length ; i++) {
				packet.b[i] = rand() % 256;
			}
			can_pwrite(STDOUT_FILENO, bin, &packet);
			usleep(delay);
		}

	} else {

		if (sscanf(argv[pos++], "%u", &(packet.id)) != 1) {
			fprintf(stderr, "Bad value for packet id\n");
			help(argv[0]);
			exit(1);
		}
		if (packet.id < 0 || packet.id > 2047) {
			fprintf(stderr, "Bad value for packet id (out of range)\n");
			help(argv[0]);
			exit(1);
		}
		packet.length = argc - pos;
		if (packet.length > 8) {
			fprintf(stderr, "Bad packet length (too long)\n");
			help(argv[0]);
			exit(1);
		}
		unsigned int b;
		for (int i = 0 ; i < packet.length ; i++) {
			sscanf(argv[pos++], "%u", &b);
			packet.b[i] = b;
		}

		while (count) {
			if (count > 0)
				count--;
			can_pwrite(STDOUT_FILENO, bin, &packet);
			usleep(delay);
		}

	}

	return EXIT_SUCCESS;
}

int getargs(int argc, char * argv[], int * delay, int * count)
{
	char options[] = "d:n:";
	int option;
	int c, d;

	while ((option = getopt(argc, argv, options)) != -1) {
		switch (option) {
			case 'd':
				if (sscanf(optarg, "%d", &d) > 0) {
					*delay = d * 1000;
				}
				break;
			case 'n':
				if (sscanf(optarg, "%d", &c) > 0) {
					*count = c;
				}
				break;
			case '?':
				exit(1);
		}
	}

	return optind;
}

