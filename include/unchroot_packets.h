#ifndef HEADER_UNCHROOT_PACKETS_H
    #define HEADER_UNCHROOT_PACKETS_H

    #include "include/types.h"
    
    extern int unchroot_packets(Buffer*, Buffer*);
    extern int packet_fxp_init(Buffer*, Buffer*);
    extern int packet_fxp_open(Buffer*, Buffer*);
    extern int packet_fxp_lstat(Buffer*, Buffer*);
    extern int packet_fxp_setstat(Buffer*, Buffer*);
    extern int packet_fxp_opendir(Buffer*, Buffer*);
    extern int packet_fxp_remove(Buffer*, Buffer*);
    extern int packet_fxp_mkdir(Buffer*, Buffer*);
    extern int packet_fxp_rmdir(Buffer*, Buffer*);
    extern int packet_fxp_realpath(Buffer*, Buffer*);
    extern int packet_fxp_stat(Buffer*, Buffer*);
    extern int packet_fxp_rename(Buffer*, Buffer*);
    extern int packet_fxp_readlink(Buffer*, Buffer*);
    extern int packet_fxp_link(Buffer*, Buffer*);
    extern int packet_fxp_extension(Buffer*, Buffer*);
#endif
