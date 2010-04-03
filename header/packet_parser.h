#ifndef HEADER_PACKET_PARSER_H
    #define HEADER_PACKET_PARSER_H
    
    #include <string.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <syslog.h>
    #include <arpa/inet.h>
    #include <sys/param.h>
    #include <errno.h>
    
    #include "include/types.h"
    #include "include/log.h"
    #include "include/buffer.h"
    #include "include/xmalloc.h"
    #include "include/cleanup.h"
    #include "include/misc.h"
    #include "include/bufaux.h"
    #include "include/chroot_packets.h"
    #include "include/unchroot_packets.h"
    
    int parse_packet(int, int, int);
    int parse_attrs(Buffer*, Buffer*, u_int*, u_int*);
#endif
