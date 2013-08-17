
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

#include "header/main.h"

char *user_homedir;
int user_homedir_len;
u_int protocol_ver;
static int from_client = 0;
static int to_client = 0;
static int to_server = 0;
static int from_server = 0;
static pid_t sftp_server_pid = 0;
static char* sftp_server = NULL;
static pthread_t inbound_thread;
static pthread_t outbound_thread;

int main(int argc, char* argv[]) {
    struct thread_parm_t tparms;
    
    memset(&tparms, 0, sizeof(struct thread_parm_t));
    memset(&inbound_thread, 0, sizeof(pthread_t));
    
    // TODO: remove hard coding of app name. Port ssh function ssh_get_progname() over to get function name
    log_init("sftp-chroot", SYSLOG_LEVEL_DEBUG3, SYSLOG_FACILITY_AUTHPRIV, 0);
    
    // Process command line arguments
    parse_cmd_arguments(argc, argv);

    // Initialise the proxy
    if (!init_proxy()) {
        exit(EXIT_FAILURE);
    }

    // Main thread
    outbound_thread = pthread_self();

    // Processing of inbound packets from the client
    tparms.to = to_server;
    tparms.from = from_client;
    tparms.direction = DIRECTION_INBOUND;
    pthread_create(&inbound_thread, NULL, (void*) &start_inbound_parser_thread, (void*) &tparms);

    // Processing of outbound pakcets from the server
    debug("Starting outbound packet parser");
    parse_packet(to_client, from_server, DIRECTION_OUTBOUND);
    debug("Finished parsing outbound packets");
    
    // Kill the other thread, close all pipes then exit
    pthread_kill(inbound_thread, SIGKILL);
    close(from_client);
    close(to_client);
    close(from_server);
    close(to_server);

    // Kill the sftp server
    kill(sftp_server_pid, SIGKILL);
    
    exit(EXIT_SUCCESS);
}

int init_proxy() {
    int server_readdes[2];
    int server_writedes[2];
    short e = 0;
    struct sigaction childact;
    memset(&childact, 0, sizeof(struct sigaction));
    struct passwd *userpwdb;
    
    // Get the users home directory, used in chrooting process
    userpwdb = getpwuid(geteuid());
    user_homedir = xstrdup(userpwdb->pw_dir);
    user_homedir_len = strlen(user_homedir);
  
    // Configure signal handling for child process termination.
    // This is important for detecting if the sftp server ends, so we can also end gracefully
    childact.sa_handler = child_signal;
    childact.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &childact, NULL);
    
    // Clone client pipe interfaces and close file descriptors with client, required for server interface
    // Our stdout is the clients stdin, and the client will send output into our stdin
    from_client = dup(STDIN_FILENO);
    to_client = dup(STDOUT_FILENO);
    
    // Connect stdin/stdout filedes numbers for communication with the sftp server process
    if (pipe(server_readdes) != 0) return 0;
    if (pipe(server_writedes) != 0) return 0;
    
    while((sftp_server_pid = fork()) < 0) {
        // Check for errors
        error("Failed to fork process attempt: %i", e);
        if (e++ > ERROR_LIMIT) return 0;
    }
    
    if (sftp_server_pid > 0) {
        // Parent process
        // WARNING: there's a futex deadlock in here somewhere for the parent, if the stdin/stdout
        // pipes do not get connected, the sftp-server process dies and the sigchild handler
        // never gets called - to fix
        
        // Update the server stdin/stdout file descriptors with server
        close (server_writedes[0]);
        close (server_readdes[1]);

        from_server = server_readdes[0];
        to_server = server_writedes[1];

    } else if (sftp_server_pid == 0) {
        // Child process
        // Close additional pipes and contiue to new process
        close (server_readdes[0]);
        close (server_writedes[1]);

	    // Ensure the stdin/stdout filedes numbers connect to our pipes
        close (STDIN_FILENO);
        close (STDOUT_FILENO);
        dup(server_writedes[0]);
        dup(server_readdes[1]);
        
        if (execl(sftp_server, NULL, (char*) 0) == -1) {
            // Failed to start sftp service. Since we're the child, exit failure
            error("Failed to start sftp server process");
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    }
    
    return 1;
}

void* start_inbound_parser_thread(void *tparms) {
    pthread_detach(pthread_self());
    
    debug("Starting inbound packet parser");
    parse_packet(((struct thread_parm_t*) tparms)->to, ((struct thread_parm_t*) tparms)->from,
        ((struct thread_parm_t*) tparms)->direction);
    debug("Finished parsing inbound packets");
    
    // Kill the other thread and close all pipes
    pthread_kill(outbound_thread, SIGKILL);
    close(from_client);
    close(to_client);
    close(from_server);
    close(to_server);
    
    // Kill the sftp server
    kill(sftp_server_pid, SIGKILL);
    
    return (void*) 0;
}

void parse_cmd_arguments(int argc, char* argv[]) {
    char f = ' ';
    int e = 0;
    opterr = 0;
   
    // Parse input, should contain "-c <path_to_sftp-server>
    while ((f = getopt(argc, argv, CMD_OPTIONS)) != -1) {
        switch (f) {
            case 'c':
                sftp_server = optarg;
                break;
            
            case ':':
                fprintf(stderr, "option -%c\n", optopt);
                e++;
                break;
            
            case '?':
            default:
                fprintf(stderr, "Unrecognised option: -%c\n", optopt);
                e++;
                break;
        }
    }
    
    // Error if -c parameter is missing
    debug("Requested to execute subsystem: '%s'", sftp_server);
    if (sftp_server == NULL || e) {
        // Missing sftp-server subsystem path
        fprintf(stderr, "Execution error: Invalid subsystem\n");
        exit(EXIT_FAILURE);
    }
}

void child_signal(int signo) {
    // Child process has ended
    debug("A child process ended, cleanup up zombies");
    pid_t p = 0;
    int status = 0;
    
    while ((p = waitpid((pid_t) -1, &status, WNOHANG)) > 0) {
        // Check if its the sftp-server and if so, exit with the same status
        if (p == sftp_server_pid) {
            error("sftp server process ended, exiting with status: %i", status);
            exit(status);
        }
    }
    
    return;
}
