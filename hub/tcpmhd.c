#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 8

int listen(int fd);

int main(int argc, char ** argv)
{
	int p[2];
	if (pipe(p) < 0) {
		perror("pipe");
		return 1;
	}
	int pid = vfork();
	switch (pid) {
		case -1: /* error */
			perror("fork");
			return 1;
			break;
		case 0: /* son */
			close(p[0]);
			if (dup2(p[1], 1) < 0) {
				perror("dup2");
				return 1;
			}
			//execlp("tcphub", "tcphub", "13756", NULL);
			
			return 1;
			break;
		default: /* father */
			close(p[1]);
			if (listen(p[0]) < 0) {
				return 1;
			}
	}

	return 0;
}

int listen(int fd)
{
	int r;
	/*FILE * file;
	if ((file = fdopen(fd, O_RDONLY)) == NULL) {
		perror("malloc");
		return -1;
	}*/
	char buffer[BUF_SIZE];
	char line[BUF_SIZE];
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	struct pollfd * pfd = NULL;
	if ((pfd = malloc(1 * sizeof(struct pollfd))) == NULL) {
		perror("malloc");
		return -1;
	}
	int b = 0;
	int w;
	int i = 0;
	while (1) {
		i = 0;
		pfd[0].fd = fd;
		pfd[0].events = POLLIN;
		pfd[0].revents = 0;
		i++;
		/*if (b > 0) {
			i++;
			pfd[1].fd = 1;
			pfd[1].events = POLLOUT;
			pfd[1].revents = 0;
		}*/
		if ((r = poll(pfd, i, -1)) <= 0) {
			if (r == 0 || errno == EINTR)
				continue;
			 perror("poll");
			 return -1;
		}
		printf("poll %d %d %d\n", r, pfd[0].revents, POLLIN);
		if (pfd[0].revents & POLLIN) {
			if ((r = read(fd, buffer, BUF_SIZE-b)) < 0) {
				perror("read");
				return -1;
			}
			printf("read %d\n", r);
			b += r;
			for (i = 0 ; i < b ; i++) {
				if (buffer[i] == '\n') {
					logline(line);
					memmove(buffer, buffer+i, i);
					b -= i;
					break;
				} else {
					line[i] = buffer[i];
				}
			}
			if (i == b) {
				logline(buffer);
				b = 0;
			}
		}
		/*if (i == 2 && pfd[1].revents & POLLOUT) {
			if ((w = write(1, buffer, r)) < 0) {
				perror("write");
			}
		}*/
	}
	perror("read");
	free(pfd);
	return -1;
}

int logline(char * line)
{
	printf("%d\t\t%s\n", 1, line);
	return 0;
}
