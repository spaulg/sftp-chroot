
/*
 Copyright 2013 Simon Paulger <spaulger@codezen.co.uk>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
