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
