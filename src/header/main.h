
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

#ifndef HEADER_MAIN_H
    #define HEADER_MAIN_H

    #include <stdlib.h>
    #include <pthread.h>
    #include <string.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <syslog.h>
    #include <signal.h>
    #include <sys/wait.h>
    #include <fcntl.h>
    #include <pwd.h>

    #include "include/types.h"
    #include "include/log.h"
    #include "include/packet_parser.h"
    #include "include/xmalloc.h"

    #define ERROR_LIMIT 3
    #define CMD_OPTIONS "c:" 
    
    int main(int, char*[]);
    int init_logging();
    int init_proxy();
    void* start_inbound_parser_thread();
    void parse_cmd_arguments(int, char*[]);
    void child_signal(int);
#endif
