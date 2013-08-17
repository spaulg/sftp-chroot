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

#ifndef HEADER_BUFFER_H
	#define HEADER_BUFFER_H

	#include <sys/param.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdarg.h>
	
	#include "include/types.h"
	#include "include/xmalloc.h"
	#include "include/log.h"
	#include "include/fatal.h"

	#define	BUFFER_MAX_CHUNK	0x100000
	#define	BUFFER_MAX_LEN		0xa00000
	#define	BUFFER_ALLOCSZ		0x008000
	
	void buffer_init(Buffer *);
	void buffer_clear(Buffer *);
	void buffer_free(Buffer *);
	
	u_int buffer_len(Buffer *);
	void *buffer_ptr(Buffer *);
	void *buffer_end(Buffer *);
	
	void buffer_append(Buffer *, const void *, u_int);
	void *buffer_append_space(Buffer *, u_int);
	
	int buffer_check_alloc(Buffer *, u_int);
	
	void buffer_get(Buffer *, void *, u_int);
	
	void buffer_consume(Buffer *, u_int);
	void buffer_consume_end(Buffer *, u_int);
	
	void buffer_dump(Buffer *);
	
	int buffer_get_ret(Buffer *, void *, u_int);
	int buffer_consume_ret(Buffer *, u_int);
	int buffer_consume_end_ret(Buffer *, u_int);
	
	//#include <openssl/bn.h>
#endif
