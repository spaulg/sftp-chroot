
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
