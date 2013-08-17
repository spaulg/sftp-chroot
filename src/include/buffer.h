/* $OpenBSD: buffer.h,v 1.17 2008/05/08 06:59:01 markus Exp $ */

/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Code for manipulating FIFO buffers.
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */

#ifndef INCLUDE_BUFFER_H
	#define INCLUDE_BUFFER_H

	#include "include/types.h"
	
	extern void buffer_init(Buffer *);
	extern void buffer_clear(Buffer *);
	extern void buffer_free(Buffer *);
	
	extern u_int buffer_len(Buffer *);
	extern void *buffer_ptr(Buffer *);
	extern void *buffer_end(Buffer *);
	
	extern void buffer_append(Buffer *, const void *, u_int);
	extern void *buffer_append_space(Buffer *, u_int);
	
	extern int buffer_check_alloc(Buffer *, u_int);
	
	extern void buffer_get(Buffer *, void *, u_int);
	
	extern void buffer_consume(Buffer *, u_int);
	extern void buffer_consume_end(Buffer *, u_int);
	
	extern void buffer_dump(Buffer *);
	
	extern int buffer_get_ret(Buffer *, void *, u_int);
	extern int buffer_consume_ret(Buffer *, u_int);
	extern int buffer_consume_end_ret(Buffer *, u_int);
#endif
