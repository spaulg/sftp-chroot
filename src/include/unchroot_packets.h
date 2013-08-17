
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
