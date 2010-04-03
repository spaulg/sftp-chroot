#ifndef HEADER_CHROOT_PACKETS_H
    #define HEADER_CHROOT_PACKETS_H

    #include <string.h>

    #include "include/types.h"
    #include "include/buffer.h"
    #include "include/bufaux.h"
    #include "include/log.h"
    #include "include/cleanup.h"
    #include "include/misc.h"
    #include "include/strlcpy.h"
    #include "include/xmalloc.h"
    #include "include/fatal.h"
    #include "include/packet_parser.h"
    
    int chroot_packets(Buffer*, Buffer*);
    int packet_fxp_version(Buffer*buff, Buffer*);
    int packet_fxp_name(Buffer*, Buffer*);
    int packet_fxp_attrs(Buffer*, Buffer*);
    u_char *chroot_filename(u_char*, const u_char*);
#endif
