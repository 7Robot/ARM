#include <stdio.h>
#define _XOPEN_SOURCE 500 /* getsubopt */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/wait.h>

#include "functions.h"

/* Default parameters */
#define DEFAULT_HOST1 "localhost"
#define DEFAULT_PORT1 "3521"
#define DEFAULT_FORMAT1 "bin"

#define DEFAULT_HOST2 "localhost"
#define DEFAULT_PORT2 "3522"
#define DEFAULT_FORMAT2 "txt"

#define CONVERTER "canconvert"

void show_help(char * cmd);
int parse_subopt(char * optarg, char ** host, char ** port, char ** format);
void close_connections(void);
void child_quit(int signal);

int pid1, pid2;

int main(int argc, char * argv[])
{
	char * env;
	int option, sock1, sock2;
	int daemon = 0;

	static char * host1 = DEFAULT_HOST1;
	static char * port1 = DEFAULT_PORT1;
	static char * format1 = DEFAULT_FORMAT1;
	char * opt_host1 = NULL;
	char * opt_port1 = NULL;
	char * opt_format1 = NULL;

	static char * host2 = DEFAULT_HOST2;
	static char * port2 = DEFAULT_PORT2;
	static char * format2 = DEFAULT_FORMAT2;
	char * opt_host2 = NULL;
	char * opt_port2 = NULL;
	char * opt_format2 = NULL;

	pid1 = 0;
	pid2 = 0;

	if (atexit(&(close_connections)) != 0) {
		perror("atexit");
		exit(1);
	}
	
	/* ENV */
	env = getenv("PONT_HOST1");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_host1 = malloc(strlen(env) + 1);
		if (opt_host1 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_host1, env);
		host1 = opt_host1;
	}
	env = getenv("PONT_HOST2");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_host2 = malloc(strlen(env) + 1);
		if (opt_host2 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_host2, env);
		host2 = opt_host2;
	}
	env = getenv("PONT_PORT1");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_port1 = malloc(strlen(env) + 1);
		if (opt_port1 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_port1, env);
		port1 = opt_port1;
	}
	env = getenv("PONT_PORT2");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_port2 = malloc(strlen(env) + 1);
		if (opt_port2 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_port2, env);
		port2 = opt_port2;
	}
	env = getenv("PONT_FORMAT1");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_format1 = malloc(strlen(env) + 1);
		if (opt_format1 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_format1, env);
		format1 = opt_format1;
	}
	env = getenv("PONT_FORMAT2");
	if ((env != NULL) && (strlen(env) != 0)) {
		opt_format2 = malloc(strlen(env) + 1);
		if (opt_format2 == NULL) {
			perror("malloc");
			exit(1);
		}
		strcpy(opt_format2, env);
		format2 = opt_format2;
	}

	/* OPT */
	opterr = 1;
	while (1) {
		option = getopt(argc, argv, "1:2:hd");
		if (option == -1)
			break;
		switch (option) {
			case '1':
				if (parse_subopt(optarg, &opt_host1, &opt_port1, &opt_format1) < 0) {
					show_help(argv[0]);
					exit(1);
				}
				if (opt_host1 != NULL)
					host1 = opt_host1;
				if (opt_port1 != NULL)
					port1 = opt_port1;
				if (opt_format1 != NULL)
					format1 = opt_format1;
				break;
			case '2':
				if (parse_subopt(optarg, &opt_host2, &opt_port2, &opt_format2) < 0) {
					show_help(argv[0]);
					exit(1);
				}
				if (opt_host2 != NULL)
					host2 = opt_host2;
				if (opt_port2 != NULL)
					port2 = opt_port2;
				if (opt_format2 != NULL)
					format2 = opt_format2;
				break;
			case 'd':
				daemon = 1;
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

	printf("Converter bridge\n");
	printf("\tExtrimity 1: %s:%s (%s)\n", host1, port1, format1);
	printf("\tExtrimity 2: %s:%s (%s)\n", host2, port2, format2);

	if ((sock1 = getsockfd(host1, port1)) < 0) {
		fprintf(stderr, "error: open_connection(%s, %s)\n", host1, port1);
		exit(1);
	}
	if ((sock2 = getsockfd(host2, port2)) < 0) {
		fprintf(stderr, "error: open_connection(%s, %s)\n", host2, port2);
		exit(1);
	}

	if (daemon) {
		chdir("/");
		if (fork() != 0) {
			exit(0);
		}
		setsid();
		if (fork() != 0) {
			exit(0);
		}
		for (int i = 0 ; i < getdtablesize() ; i++) { // FIXME
			if (i != sock1 && i != sock2)
				close(i);
		}
	}

	signal(SIGCHLD, child_quit);

	switch ((pid1 = fork())) {
		case -1:
			perror("fork");
			exit(1);
		case 0:
			dup2(sock1, STDIN_FILENO);
			dup2(sock2, STDOUT_FILENO);
			execlp(CONVERTER, CONVERTER, "-i", format1, "-o", format2, NULL);
			perror("eveclp");
			exit(1);
	}

	switch ((pid2 = fork())) {
		case -1:
			kill(pid1, SIGTERM);
			perror("fork");
			exit(1);
		case 0:
			dup2(sock2, STDIN_FILENO);
			dup2(sock1, STDOUT_FILENO);
			execlp(CONVERTER, CONVERTER, "-i", format2, "-o", format1, NULL);
			perror("eveclp");
			exit(1);
	}

	pause();

	return 0;
}

void show_help(char * cmd)
{
	printf("Usage: %s [-(1|2) host=h,port=p,format=(bin|dec|hex)] [-d]\n", cmd);
}

int parse_subopt(char * optarg, char ** host, char ** port, char ** format)
{
	int subopt;
	char * ssopt = optarg;
	char * value = NULL;

	char * tokens[] = {
		"host", "port", "format", NULL
	};

	while ((subopt = getsubopt(&ssopt, tokens, &value)) != -1) {
		switch (subopt) {
			case 0: /* Host */
				if (value == NULL) {
					fprintf(stderr, "Host expected\n");
					return -1;
				}
				if ((*host = realloc(*host, strlen(value) + 1)) == NULL) {
					perror("realloc");
					exit(-1);
				}
				strcpy(*host, value);
				break;
			case 1: /* Port */
				if (value == NULL) {
					fprintf(stderr, "Port expected\n");
					return -1;
				}
				if ((*port = realloc(*port, strlen(value) + 1)) == NULL) {
					perror("realloc");
					exit(-1);
				}
				strcpy(*port, value);
				break;
			case 2: /* Port */
				if (value == NULL) {
					fprintf(stderr, "Format expected\n");
					return -1;
				}
				if ((*format = realloc(*format, strlen(value) + 1)) == NULL) {
					perror("realloc");
					exit(-1);
				}
				strcpy(*format, value);
				break;
		}
	}
	return 0;
}

void close_connections(void)
{
	if (pid1 != 0)
		kill(pid1, SIGTERM);
	if (pid2 != 0)
		kill(pid2, SIGTERM);
}

void child_quit(int signal)
{
	int status;
	int pid = wait(&status);
	if (pid == pid1)
		kill(pid2, SIGTERM);
	else
		kill(pid1, SIGTERM);
	exit(status);
}
