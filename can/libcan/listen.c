/*
 * libcan - CAN library
 * 
 * Copyright (C) 2012 7Robot <7robot@list.bde.enseeiht.fr>
 * Wrotten by Élie Bouttier <elie.bouttier@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "libcan.h"
#include "libcan-private.h"

typedef struct can_t can_t;

void dispatch(can_ctx * ctx, can_t packet);
void * listen(void * arg);
void * bin_listener(can_ctx * ctx);
void * dec_listener(can_ctx * ctx);
void * hex_listener(can_ctx * ctx);

int can_register_callback(can_ctx * ctx, int mask, int filter, void (*cbf)(can_t))
{
	int i;
	
	for (i = 0 ; i < ctx->asize ; i++) {
		if (ctx->cbfcts[i] == NULL)
			break;
	}
	if (i == ctx->asize) { /* End of array reached without finding empty cell */
		ctx->cbfcts = realloc(ctx->cbfcts, (ctx->asize + ctx->inc) * sizeof(struct can_cbf));
		if (ctx->cbfcts == NULL)
			return -1;
		ctx->asize += ctx->inc;
		for (int j = i ; j < ctx->asize ; j++) {
			ctx->cbfcts[j] = NULL;
		}
	}
	if ((ctx->cbfcts[i] = calloc(1, sizeof(struct can_cbf))) == NULL)
		return -1;
	ctx->cbfcts[i]->mask = mask;
	ctx->cbfcts[i]->filter = filter;
	ctx->cbfcts[i]->fct = cbf;
	
	return 0;
}

int can_unregister_callback(can_ctx * ctx, int id)
{
	if (id >= ctx->asize || ctx->cbfcts[id] == NULL) {
		errno = EINVAL;
		return -1;
	}
	free(ctx->cbfcts[id]);
	ctx->cbfcts[id] = NULL;
	return 0;
}

void can_on_error(can_ctx * ctx, void (*on_error)())
{
	ctx->on_error = on_error;
}

int can_listen_on(can_ctx * ctx, int fd, enum can_f format)
{
	if (ctx->status != 0) {
		pthread_cancel(ctx->pth);
		ctx->status = 0;
		ctx->fd = 0;
	}
	if (fd >= 0) {
		int flags;
		if ((flags = fcntl(fd, F_GETFL)) < 0)
			return -1;
		if (fcntl(fd, F_SETFL, flags & ~O_NONBLOCK) < 0)
			return -1;
		ctx->fd = fd;
		ctx->format = format;
		if ((errno = pthread_create(&(ctx->pth), NULL, listen, (void *) ctx)) < 0)
			return -1;
	}
	return 0;
}

void dispatch(can_ctx * ctx, can_t packet)
{
	int i;
	struct can_cbf * cbf;
	
	for (i = 0 ; i < ctx->asize ; i++) {
		cbf = ctx->cbfcts[i];
		if (cbf != NULL) {
			if ((packet.id & cbf->mask) == cbf->filter) {
				cbf->fct(packet);
			} 
		}
	}
}

void * listen(void * arg)
{
	can_ctx * ctx = (can_ctx *) arg;
	ctx->status = 1; /* listening */
	switch(ctx->format) {
		case bin:
			bin_listener(ctx);
			break;
		case txt:
		case dec:
			dec_listener(ctx);
			break;
		case hex:
			hex_listener(ctx);
			break;
		default:
			fprintf(stderr, "Error: unsupported input format.\n");
	}
	ctx->status = 0;
	if (ctx->on_error != NULL) {
		ctx->on_error();
	}
	return NULL;
}


void * bin_listener (can_ctx * ctx)
{
	can_t packet;
	uint8_t c;

	/*
	 * state = 1 : waiting FD
	 * state = 2 : waiting length + ID high
	 * state = 3 : waiting ID low
	 * state = 0 : waiting BF
	 * state = -k : waiting k bytes of data
	 */
	int state = 1; /* waiting FD */
	
	while (1) {
		if (read(ctx->fd, &c, 1) <= 0) {
			return NULL;
		}
		if (state == 1) { /* waiting FD */
			if (c == 0xFD) {
				state = 2; /* waiting length + id high */
			}
		} else if (state == 2) { /* waiting length + id high */
			packet.length = c>>4;
			if (packet.length > 8) {
				state = 1; /* waiting FD */
			} else {
				packet.id = (c%16)<<8;
				state = 3; /* waiting id low */
			}
		} else if (state == 3) {
			packet.id |= c;
			state = -packet.length;
		} else if (state == 0) { /* waiting BF */
			if (c == 0xBF) {
				dispatch(ctx, packet);
				state = 1; /* waiting FD */
			} else {
				state = 1; /* waiting FD */
			}
		} else {
			packet.b[packet.length + state] = c;
			state++;
		}
	}
	
	return NULL;
}

void * dec_listener (can_ctx * ctx)
{
	can_t packet;
	char c;
	int state = 0;
	int n = 0;
	FILE * stream;
	char line[128];

	if ((stream = fdopen(ctx->fd, "r")) == NULL) {
		perror("fdopen");
	}

	while (1) {
		fgets(line, 128, stream);
		packet.length = sscanf(line, "%d%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu%*[ \t]%hhu", &packet.id,
				packet.b, packet.b+1, packet.b+2, packet.b+3, packet.b+4, packet.b+5, packet.b+6, packet.b+7) - 1;
		if (packet.length >= 0 && packet.id < 2048) {
			dispatch(ctx, packet);
		}
	}
	return NULL;
}

void * hex_listener (can_ctx * ctx)
{
	can_t packet;
    char c;
    int state = 0;
    int n = 0;
    FILE * stream;
    char line[128];

    if ((stream = fdopen(ctx->fd, "r")) == NULL) {
        perror("fdopen");
    }

    while (1) {
        fgets(line, 128, stream);
        packet.length = sscanf(line, "%d%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX%*[ \t]%hhX", &packet.id,
                packet.b, packet.b+1, packet.b+2, packet.b+3, packet.b+4, packet.b+5, packet.b+6, packet.b+7) - 1;
        if (packet.length >= 0 && packet.id < 2048) {
            dispatch(ctx, packet);
        }
    }
    return NULL;
}
