#ifndef INCLUDE_TYPES_H
    #define INCLUDE_TYPES_H
    
    #include <sys/types.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <pthread.h>
    #include "config.h"
    
    /* Maximum packet that we are willing to send/accept */
    #define SFTP_MAX_MSG_LENGTH	(256 * 1024)
    
    #if !defined(HAVE_ATTRIBUTE__BOUNDED__) && !defined(__bounded__)
	#define __bounded__(x, y, z)
    #endif

    #ifndef SIZE_T_MAX
        #define SIZE_T_MAX ULONG_MAX
    #endif /* SIZE_T_MAX */
    
    #ifndef HAVE_SIZE_T
        typedef unsigned int size_t;
        # define HAVE_SIZE_T
        # define SIZE_T_MAX UINT_MAX
    #endif /* HAVE_SIZE_T */

    // An SFTP packet
    typedef struct {
	u_char	*buf;		/* Buffer for data. */
	u_int	 alloc;		/* Number of bytes allocated for data. */
	u_int	 offset;	/* Offset of first byte containing data. */
	u_int	 end;		/* Offset of last byte containing data. */
    } Buffer;

    struct thread_parm_t {
        int to;
	int from;
	int direction;
    };

    #define SSH_FXP_INIT                	1
    #define SSH_FXP_VERSION             	2
    #define SSH_FXP_OPEN                	3
    #define SSH_FXP_CLOSE               	4
    #define SSH_FXP_READ                	5
    #define SSH_FXP_WRITE               	6
    #define SSH_FXP_LSTAT               	7
    #define SSH_FXP_FSTAT               	8
    #define SSH_FXP_SETSTAT             	9
    #define SSH_FXP_FSETSTAT           		10
    #define SSH_FXP_OPENDIR            		11
    #define SSH_FXP_READDIR            		12
    #define SSH_FXP_REMOVE             		13
    #define SSH_FXP_MKDIR              		14
    #define SSH_FXP_RMDIR              		15
    #define SSH_FXP_REALPATH           		16
    #define SSH_FXP_STAT               		17
    #define SSH_FXP_RENAME             		18
    #define SSH_FXP_READLINK           		19
    #define SSH_FXP_SYMLINK			20

    #define SSH_FXP_STATUS            		101
    #define SSH_FXP_HANDLE            		102
    #define SSH_FXP_DATA              		103
    #define SSH_FXP_NAME              		104
    #define SSH_FXP_ATTRS             		105

    #define SSH_FXP_EXTENDED          		200
    #define SSH_FXP_EXTENDED_REPLY    		201
    
    #define SSH_FILEXFER_ATTR_SIZE    		0x00000001
    #define SSH_FILEXFER_ATTR_UIDGID		0x00000002
    #define SSH_FILEXFER_ATTR_PERMISSIONS       0x00000004
    #define SSH_FILEXFER_ATTR_ACMODTIME		0x00000008
    #define SSH_FILEXFER_ATTR_EXTENDED          0x80000000
    
    #define DIRECTION_INBOUND		1
    #define DIRECTION_OUTBOUND		2
    
    extern char *user_homedir;
    extern int user_homedir_len;
    extern u_int protocol_ver;
#endif
