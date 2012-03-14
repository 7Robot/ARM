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
#ifndef _LIBCAN_H_
#define _LIBCAN_H_

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
    namespace libcan {
#endif

/***********************
 * CAN library context *
 ***********************/

/** Opaque object representing the library context. */
typedef struct can_ctx can_ctx;
/** Create can context library. */
int can_init(struct can_ctx ** ctx);
/** Take a reference of the can library context. */
struct can_ctx * can_ref(struct can_ctx * ctx);
/** Drop a reference of the can library context. If the refcount reaches zero,
 * the ressources of the context will be released. */
int can_unref(struct can_ctx * ctx);


/**************************************************
 * CAN packet representative structure and format *
 **************************************************/

/** Transparent object reprensenting a CAN packet. */
struct can_t {
    int id; /* ID (from 0 to 2047) */
    int length; /* Packet's length (from 0 to 8) */
    uint8_t b[8]; /* Packet's data bytes */
};

/** Enumeration of can bus data format. */
enum can_f {
    bin, /* [FD] [lenth(4b)|0|id(3)] [id] [byte1] [byte2] … [byte8] [BF] */
    dec, /* 1235  12 23 42 */
    hex, /* 1235  0C 17 2A */
    txt /* significative string */
};


/***********************
 * CAN packet emission *
 ***********************/

/** Write a can packet in specified file descriptor in specified format. */
int can_write(int fd, int format, int id, int length, ...);

/** Write a can packet in specified file descriptor in specified format. */
int can_pwrite(int fd, int format, struct can_t const * packet);

/** Write a can packet in specified file descriptor in specified format. */
int can_vwrite(int fd, int format, int id, int length, va_list ap);

/************************
 * CAN packet reception *
 ************************/

/** Register a callback function on can packet reception.
 * The function will be call only for packet where id & mask = filter.
 * Return index of callback function, necessary for unregister it. */
int can_register_callback(can_ctx * ctx,
        int mask, int filter, void (*cbf)(struct can_t));
/** Unregister a callback function. */
int can_unregister_callback(can_ctx * ctx, int id);
/** Run listener thread on a file descriptor. */
int can_listen_on(can_ctx * ctx, int fd, enum can_f format);
/** Register callback function called on listening error */
void can_on_error(can_ctx * ctx, void (*on_error)());


#ifdef __cplusplus
    } /* namespace libcan */
} /* extern "C" */
#endif

#endif
