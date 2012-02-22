#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "functions.h"
#include "MissionHandler.h"
#include "Can.h"

#define DEFAULT_HOST "r2d2"
#define DEFAULT_PORT "7771"
#define DEFAULT_MISSION "demo"
#define DEFAULT_MISSION_DIRECTORY "../"

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

	Can can;

	printf("Create mission handler …\n");
	MissionHandler mh(mission_directory, &can);
	
	printf("Open bus can …\n");
	if ((canbus = getsockfd(host, port)) < 0) {
		fprintf(stderr, "error: getsockfd(%s, %s) failed", host, port);
		return 1;
	}
	can.setup(canbus, MissionHandler::recv);

	if (mh.load(mission)) {
		fprintf(stderr, "error: failed to load mission « %s »\n", mission);
		return 1;
	}

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
