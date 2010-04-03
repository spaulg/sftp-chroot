#ifndef INCLUDE_MISC_H
    #define INCLUDE_MISC_H
    
    /* Functions to extract or store big-endian words of various sizes */
    extern u_int64_t get_u64(const void *) __attribute__((__bounded__( __minbytes__, 1, 8)));
    extern u_int32_t get_u32(const void *) __attribute__((__bounded__( __minbytes__, 1, 4)));
    extern u_int16_t get_u16(const void *) __attribute__((__bounded__( __minbytes__, 1, 2)));
    extern u_int8_t get_u8(const void *) __attribute__((__bounded__( __minbytes__, 1, 1)));
    extern u_char* get_string(const void*, u_int*);
    extern void put_u64(void *, u_int64_t) __attribute__((__bounded__( __minbytes__, 1, 8)));
    extern void put_u32(void *, u_int32_t) __attribute__((__bounded__( __minbytes__, 1, 4)));
    extern void put_u16(void *, u_int16_t) __attribute__((__bounded__( __minbytes__, 1, 2)));
    extern void put_u8(void *, u_int8_t) __attribute__((__bounded__( __minbytes__, 1, 1)));
#endif
