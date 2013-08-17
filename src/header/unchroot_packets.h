
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

#ifndef HEADER_UNCHROOT_PACKETS_H
    #define HEADER_UNCHROOT_PACKETS_H

    #include <string.h>
    #include <unistd.h>
    #include <limits.h>
    #include <stdlib.h>
    #include <errno.h>

    #include "include/types.h"
    #include "include/buffer.h"
    #include "include/bufaux.h"
    #include "include/log.h"
    #include "include/cleanup.h"
    #include "include/misc.h"
    #include "include/xmalloc.h"
    #include "include/strlcpy.h"
    #include "include/packet_parser.h"
    #include "include/fatal.h"
    
    int unchroot_packets(Buffer*, Buffer*);
    int packet_fxp_init(Buffer*, Buffer*);
    int packet_fxp_open(Buffer*, Buffer*);
    int packet_fxp_setstat(Buffer*, Buffer*);
    int packet_fxp_fsetstat(Buffer*, Buffer*);
    int packet_fxp_mkdir(Buffer*, Buffer*);
    int packet_fxp_realpath(Buffer*, Buffer*);
    int packet_fxp_rename(Buffer*, Buffer*);
    int packet_fxp_readlink(Buffer*, Buffer*);
    int packet_fxp_symlink(Buffer*, Buffer*);
    int packet_fxp_extension(Buffer*, Buffer*);
    
    int packet_fxp_basicpath(Buffer*, Buffer*);
    
    u_char *unchroot_filename(u_char*, const u_char*);
#endif
