#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "functions.h"
#include "MissionHandler.h"
#include "Spread.h"
#include "Can.h"

#include "Queue.h"
#include "Task.h"

#include "TaskLoad.h"

#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT "7771"
#define DEFAULT_MISSION "petit"
#define DEFAULT_MISSION_DIRECTORY "./"

void show_help(char * cmd);

int main(int argc, char ** argv)
{
	char * env;
	int option, canbus;

	static char * host = (char*)DEFAULT_HOST;
	char * opt_host = NULL;

    static char * port = (char*)DEFAULT_PORT;
    char * opt_port = NULL;

    static char * mission = (char*)DEFAULT_MISSION;
    char * opt_mission = NULL;

    static char * mission_directory = (char*)DEFAULT_MISSION_DIRECTORY;
    char * opt_mission_directory = NULL;

	/* ENV */
    env = getenv("IA_HOST");
    if ((env != NULL) && (strlen(env) != 0)) {
        opt_host = (char*)malloc(strlen(env) + 1);
        if (opt_host == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(opt_host, env);
        host = opt_host;
    }
    env = getenv("IA_PORT");
    if ((env != NULL) && (strlen(env) != 0)) {
        opt_port = (char*)malloc(strlen(env) + 1);
        if (opt_port == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(opt_port, env);
        port = opt_port;
    }
    env = getenv("IA_DIR");
    if ((env != NULL) && (strlen(env) != 0)) {
        opt_mission_directory = (char*)malloc(strlen(env) + 1);
        if (opt_mission_directory == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(opt_mission_directory, env);
        mission_directory = opt_mission_directory;
    }
    env = getenv("IA_MISSION");
    if ((env != NULL) && (strlen(env) != 0)) {
        opt_mission = (char*)malloc(strlen(env) + 1);
        if (opt_mission == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(opt_mission, env);
        mission = opt_mission;
    }

    /* OPT */
    opterr = 1;
    while (1) {
        option = getopt(argc, argv, "H:P:M:D:h");
        if (option == -1)
            break;
        switch (option) {
            case 'H':
                opt_host = (char*)malloc(strlen(optarg) + 1);
                if (opt_host == NULL) {
                    perror("malloc");
                    exit(1);
                }
                strcpy(opt_host, optarg);
                host = opt_host;
                break;
            case 'P':
                opt_port = (char*)malloc(strlen(optarg) + 1);
                if (opt_port == NULL) {
                    perror("malloc");
                    exit(1);
                }
                strcpy(opt_port, optarg);
                port = opt_port;
                break;
            case 'M':
                opt_mission = (char*)malloc(strlen(optarg) + 1);
                if (opt_mission == NULL) {
                    perror("malloc");
                    exit(1);
                }
                strcpy(opt_mission, optarg);
                mission = opt_mission;
                break;
            case 'D':
                opt_mission_directory = (char*)malloc(strlen(optarg) + 2);
                if (opt_mission_directory == NULL) {
                    perror("malloc");
                    exit(1);
                }
                strcpy(opt_mission_directory, optarg);
				strcat(opt_mission_directory, "/");
                mission_directory = opt_mission_directory;
                break;
            case 'h':
                show_help(argv[0]);
                exit(0);
            case '?':
                show_help(argv[0]);
                exit(1);
            default:
                break;
        }
    }

	printf("IA Settings\n");
	printf("\tBus CAN: %s:%s\n", host, port);
	printf("\tStarting mission: %s\n", mission);
	printf("\tMissions directory: %s\n\n", mission_directory);


	//printf("Create mission handler …\n");
	//MissionHandler mh(mission_directory, &can);
	
	printf("Open bus can …\n");
	if ((canbus = getsockfd(host, port)) < 0) {
		fprintf(stderr, "error: getsockfd(%s, %s) failed\n", host, port);
		return 1;
	}
	Can can(canbus);

	MissionHandler::setup(mission_directory, &can);

	/////////////////////////////////////////////////////////////////////

	/*for (int i = 0 ; i < 5 ; i++) {
		Task * t = new TaskLoad(mission, NULL);
		printf("%p\n", t);
		Queue::push(t);
	}
	for (int i = 0 ; i < 5 ; i++) {
		printf("%p\n", Queue::tasks->top());
		Queue::tasks->pop();
	}*/
	Queue::push(new TaskLoad(mission, NULL));

	Queue::start();
	Queue::wait();
	
	/*if (MissionHandler::handler(mission)) {
		fprintf(stderr, "MissionHandler::handler failed\n");
		return 1;
	}*/

	/////////////////////////////////////////////////////////////////////

	return 0;
}

void show_help(char * cmd)
{
    printf("Usage: %s [-H <host>] [-P <port>] [-M <mission>] [-D <missions directory>] [-h]\n", cmd);
    printf("\tH: set host to connect\n");
    printf("\tP: set port to connect\n");
    printf("\tM: set mission to start\n");
    printf("\tD: set directory that contains missions\n");
    printf("\th: show this help\n");
}
