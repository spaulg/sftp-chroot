#ifndef HEADER_MISC_H
    #define HEADER_MISC_H
    
    #include <sys/types.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <sys/param.h>
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <netdb.h>
    
    #include "include/xmalloc.h"
    #include "include/log.h"
    #include "include/types.h"
    
    /* Functions to extract or store big-endian words of various sizes */
    u_int64_t get_u64(const void *) __attribute__((__bounded__( __minbytes__, 1, 8)));
    u_int32_t get_u32(const void *) __attribute__((__bounded__( __minbytes__, 1, 4)));
    u_int16_t get_u16(const void *) __attribute__((__bounded__( __minbytes__, 1, 2)));
    u_int8_t get_u8(const void *) __attribute__((__bounded__( __minbytes__, 1, 1)));
    u_char* get_string(const void*, u_int*);
    void put_u64(void *, u_int64_t) __attribute__((__bounded__( __minbytes__, 1, 8)));
    void put_u32(void *, u_int32_t) __attribute__((__bounded__( __minbytes__, 1, 4)));
    void put_u16(void *, u_int16_t) __attribute__((__bounded__( __minbytes__, 1, 2)));
    void put_u8(void *, u_int8_t) __attribute__((__bounded__( __minbytes__, 1, 1)));
#endif
