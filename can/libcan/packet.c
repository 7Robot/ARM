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

 #include <unistd.h>
 #include <stdarg.h>
 #include <string.h>
 #include <errno.h>
 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 
 #include "libcan.h"
 #include "libcan-private.h"

 #define DEFAULT_ID_FORMAT_FILE "/etc/canconvert/can-messages.txt"
 #define DEFAULT_ID_FORMAT_ENV "CAN_MESSAGES_FILE"

static int bin_write(int fd, struct can_t const * packet);
static int dec_write(int fd, struct can_t const * packet);
static int hex_write(int fd, struct can_t const * packet);
static int txt_write(int fd, struct can_t const * packet);


/** Write a can packet in specified file descriptor in specified format. */
int can_write(int fd, int format, int id, int length, ...)
{
    va_list ap;
	
	va_start(ap, length);
	int rvalue = can_vwrite(fd, format, id, length, ap);
	va_end(ap);

	return rvalue;
}

/** Write a can packet in specified file descriptor in specified format. */
int can_pwrite(int fd, int format, struct can_t const * packet)
{
	switch (format) {
		case bin:
			return bin_write(fd, packet);
		case dec:
			return dec_write(fd, packet);
		case hex:
			return hex_write(fd, packet);
		case txt:
			return txt_write(fd, packet);
		default:
			fprintf(stderr, "Error: unsupported output format.\n");
			return -1;
	}
}

/** Write a can packet in specified file descriptor in specified format. */
int can_vwrite(int fd, int format, int id, int length, va_list ap)
{
    int i;
	int b;
	struct can_t packet;

	if (id > 2048 || id < 0) {
		errno = EINVAL;
		return -1;
	}
	packet.id = id;

	if (length > 8 || length < 0) {
		errno = EINVAL;
		return -1;
	}
	packet.length = length;
	
	for (i = 0 ; i < length ; i++) {
		b = (uint8_t) va_arg(ap, int);
		if (b > 255 || b < 0) {
			errno = EINVAL;
			return -1;
		}
		packet.b[i] = b;
	}
	
	return can_pwrite(fd, format, &packet);
}

static int bin_write(int fd, struct can_t const * packet)
{
	int i;
	int id;
	static uint8_t * raw = NULL;
	if (raw == NULL) {
		if ((raw = malloc(12 * sizeof(uint8_t))) == NULL) {
			errno = EINVAL;
			return -1;
		}
	}
	raw[0] = 0xFD;
	id = packet->id;
	raw[1] = id >> 8;
	id &= 0xFF;
	raw[2] = id;
	raw[1] |= packet->length << 4;
	for (i = 0 ; i < packet->length ; i++) {
		raw[i+3] = packet->b[i];
	}
	raw[i+3] = 0xBF;
	if (write(fd, raw, i+4) == -1) {
		return -1;
	}
	fsync(fd);

	return 0;
}

static int dec_write(int fd, struct can_t const * packet)
{
	char output[128];
	sprintf(output, "%-6i", packet->id);
	for (int i = 0 ; i < packet->length ; i++) {
		sprintf(output+strlen(output), "   %-3i", packet->b[i]);
	}
	sprintf(output+strlen(output), "\n");
	write(fd, output, strlen(output));
	fsync(fd);

	return 0;
}

static int hex_write(int fd, struct can_t const * packet)
{
	char output[128];
	sprintf(output, "%-6i", packet->id);
	for (int i = 0 ; i < packet->length ; i++) {
		sprintf(output+strlen(output), "   %02X", packet->b[i]);
	}
	sprintf(output+strlen(output), "\n");
	write(fd, output, strlen(output));
	fsync(fd);

	return 0;
}

static int txt_write(int fd, struct can_t const * packet)
{
	char output[256];
	int i;

	sprintf(output, "%-6i", packet->id);
	for (i = 0 ; i < packet->length ; i++) {
		sprintf(output+strlen(output), "   %-3i", packet->b[i]);
	}
	for (; i < 9 ; i++) {
		sprintf(output+strlen(output), "      ", packet->b[i]);
	}

	/* Traduction */
	char traduc[128];
	FILE * format;
	static char * filename = DEFAULT_ID_FORMAT_FILE;
	static int check_env = 0;
	if (!check_env) {
		char * opt_filename = NULL;
		char * env;
		env = getenv(DEFAULT_ID_FORMAT_ENV);
		if ((env != NULL) && (strlen(env) != 0)) {
			opt_filename = malloc(strlen(env)+1);
			if (opt_filename == NULL) {
				perror("malloc");
			} else {
				strcpy(opt_filename, env);
				filename = opt_filename;
			}
			check_env = 1;
		}
	}
	if ((format = fopen(filename, "r")) == NULL) {
		perror("fopen");
		fprintf(stderr, "Error: unable to open id traduction file.\n");
	} else {
		int id;
		while(fscanf(format, "%i", &id) > 0) {
			char next = fgetc(format);
			if ((packet->id == id) && ((next == ' ') || (next == '\t'))) {
				//fscanf(format, "%*[^\n]\n", traduc);
				fscanf(format, "%s\n", traduc);
				sprintf(output+strlen(output), "  \t%s", traduc);
				break;
			} else {
				//fscanf(format, "%*[^\n]\n", traduc);
				fscanf(format, "%s\n", traduc);
			}
		}
		fclose(format);
	}

	sprintf(output+strlen(output), "\n");
	write(fd, output, strlen(output));
	fsync(fd);

	return 0;
}
