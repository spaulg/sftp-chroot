#ifndef INCLUDE_CHROOT_PACKETS_H
    #define INCLUDE_CHROOT_PACKETS_H

    #include "include/types.h"
    
    extern int chroot_packets(Buffer*, Buffer*);
    extern int packet_fxp_version(Buffer*buff, Buffer*);
    extern int packet_fxp_name(Buffer*, Buffer*);
#endif
