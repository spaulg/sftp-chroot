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

#ifndef INCLUDE_BUFAUX_H
	#define INCLUDE_BUFAUX_H
	
	#include <openssl/bn.h>
	
	extern void buffer_put_bignum(Buffer *, const BIGNUM *);
	extern void buffer_put_bignum2(Buffer *, const BIGNUM *);
	extern void buffer_get_bignum(Buffer *, BIGNUM *);
	extern void buffer_get_bignum2(Buffer *, BIGNUM *);
	
	extern u_short	buffer_get_short(Buffer *);
	extern void buffer_put_short(Buffer *, u_short);
	
	extern u_int buffer_get_int(Buffer *);
	extern void buffer_put_int(Buffer *, u_int);
	
	extern u_int64_t buffer_get_int64(Buffer *);
	extern void buffer_put_int64(Buffer *, u_int64_t);
	
	extern int buffer_get_char(Buffer *);
	extern void buffer_put_char(Buffer *, int);
	
	extern void *buffer_get_string(Buffer *, u_int *);
	extern void *buffer_get_string_ptr(Buffer *, u_int *);
	extern void buffer_put_string(Buffer *, const void *, u_int);
	extern void buffer_put_cstring(Buffer *, const char *);
	
	extern int buffer_put_bignum_ret(Buffer *, const BIGNUM *);
	extern int buffer_get_bignum_ret(Buffer *, BIGNUM *);
	extern int buffer_put_bignum2_ret(Buffer *, const BIGNUM *);
	extern int buffer_get_bignum2_ret(Buffer *, BIGNUM *);
	extern int buffer_get_short_ret(u_short *, Buffer *);
	extern int buffer_get_int_ret(u_int *, Buffer *);
	extern int buffer_get_int64_ret(u_int64_t *, Buffer *);
	extern void *buffer_get_string_ret(Buffer *, u_int *);
	extern int buffer_get_char_ret(char *, Buffer *);
#endif
