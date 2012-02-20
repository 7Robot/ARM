#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "functions.h"

#include "Robot.h"

#define DEFAULT_HOST "petit"
#define DEFAULT_PORT "3521"

void show_help(char * cmd);

Robot * robot;

int main(int argc, char ** argv)
{
	char * env;
	int option, canbus;

	static char * host = (char*)DEFAULT_HOST;
	char * opt_host = NULL;

    static char * port = (char*)DEFAULT_PORT;
    char * opt_port = NULL;

    env = getenv("TTYCLIENT_HOST");
    if ((env != NULL) && (strlen(env) != 0)) {
        opt_host = (char*)malloc(strlen(env) + 1);
        if (opt_host == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(opt_host, env);
        host = opt_host;
    }
    env = getenv("TTYCLIENT_PORT");
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
        option = getopt(argc, argv, "H:P:h");
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

	if ((canbus = getsockfd(host, port)) < 0) {
		fprintf(stderr, "error: getsockfd(%s, %s) failed", host, port);
	}

	printf("Create Robot …\n");

	Robot r(canbus);

	robot = &r;

	r.start();
}

void show_help(char * cmd)
{
    printf("Usage: %s [-H <host>] [-P <port>] [-h]\n", cmd);
    printf("\tH: set host to connect\n");
    printf("\tP: set port to connect\n");
    printf("\th: show this help\n");
}
