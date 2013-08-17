
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

#include "header/packet_parser.h"

int parse_packet(int to, int from, int direction) {
    Buffer buff, preped_buff;
    char buf[4*4096];
    int max;
    ssize_t len, set_size, olen;
    fd_set *rset, *wset;
 
    memset(&buff, 0, sizeof(Buffer));
    memset(&preped_buff, 0, sizeof(Buffer));
    
    buffer_init(&buff);
    buffer_init(&preped_buff);
    
    max = 0;
    if (to > max)
	max = to;
    if (from > max)
	max = from;
    
    set_size = howmany(max + 1, NFDBITS) * sizeof(fd_mask);
    rset = (fd_set *)xmalloc(set_size);
    wset = (fd_set *)xmalloc(set_size);
    
    while (1) {
        memset(rset, 0, set_size);
        memset(wset, 0, set_size);

        // If the input buffer has data
        if (buffer_check_alloc(&buff, sizeof(buf)))
            FD_SET(from, rset);

        // If the output buffer has data
        olen = buffer_len(&preped_buff);
        if (olen > 0)
            FD_SET(to, wset);

        if (select(max+1, rset, wset, NULL, NULL) < 0) {
            if (errno == EINTR)
                continue;
            error("select: %s", strerror(errno));
            cleanup_exit(2);
        }

        // If stdin has sent some data, read it in
        if (FD_ISSET(from, rset)) {
            // stdin ready for reading
            len = read(from, buf, sizeof buf);
            if (len == 0) {
                debug("read eof");
                cleanup_exit(0);

            } else if (len < 0) {
                error("read: %s", strerror(errno));
                cleanup_exit(1);

            } else {
                buffer_append(&buff, buf, len);
            }
        }

        // Rewrite packets if a complete packet has been received
        if (buffer_check_alloc(&buff, SFTP_MAX_MSG_LENGTH)) {
            if (direction == DIRECTION_INBOUND)
                unchroot_packets(&buff, &preped_buff);

            if (direction == DIRECTION_OUTBOUND)
                chroot_packets(&buff, &preped_buff);
        }

        // If the destination is ready for consumption, send the preped buffer out
        if (FD_ISSET(to, wset)) {
            olen = buffer_len(&preped_buff);
            len = write(to, buffer_ptr(&preped_buff), olen);
            if (len < 0) {
                error("write: %s", strerror(errno));
                cleanup_exit(1);

            } else {
                buffer_consume(&preped_buff, len);
            }
        }
    }
    
    return 1;
}

int parse_attrs(Buffer *buff, Buffer *preped_buff, u_int *msg_len, u_int *remaining_msg_len) {
    // File attributes
    u_int x;
    u_int attr_flags, xattr_flags;
    u_int64_t attr_int64;
    u_int attr_int32;
    u_char *attr_string;
    u_int attr_string_len;

    // Need to analyse attributes flags and copy flags over accordingly
    attr_flags = buffer_get_int(buff);
    xattr_flags = attr_flags;
    
    if ((attr_flags & SSH_FILEXFER_ATTR_EXTENDED) == SSH_FILEXFER_ATTR_EXTENDED)
        attr_flags = attr_flags ^ SSH_FILEXFER_ATTR_EXTENDED;
        
    buffer_put_int(preped_buff, attr_flags);
    *remaining_msg_len -= 4;
    
    
    if ((xattr_flags & SSH_FILEXFER_ATTR_SIZE) == SSH_FILEXFER_ATTR_SIZE) {
        attr_int64 = buffer_get_int64(buff);
        buffer_put_int64(preped_buff, attr_int64);
        *remaining_msg_len -= 8;
    }
    
    if ((xattr_flags & SSH_FILEXFER_ATTR_UIDGID) == SSH_FILEXFER_ATTR_UIDGID) {
        // uid
        attr_int32 = buffer_get_int(buff);
        buffer_put_int(preped_buff, attr_int32);
        *remaining_msg_len -= 4;

        // gid
        attr_int32 = buffer_get_int(buff);
        buffer_put_int(preped_buff, attr_int32);
        *remaining_msg_len -= 4;
    }
    
    if ((xattr_flags & SSH_FILEXFER_ATTR_PERMISSIONS) == SSH_FILEXFER_ATTR_PERMISSIONS) {
        attr_int32 = buffer_get_int(buff);
        buffer_put_int(preped_buff, attr_int32);
        *remaining_msg_len -= 4;
    }
    
    if ((xattr_flags & SSH_FILEXFER_ATTR_ACMODTIME) == SSH_FILEXFER_ATTR_ACMODTIME) {
        attr_int32 = buffer_get_int(buff);
        buffer_put_int(preped_buff, attr_int32);
        *remaining_msg_len -= 4;

        attr_int32 = buffer_get_int(buff);
        buffer_put_int(preped_buff, attr_int32);
        *remaining_msg_len -= 4;
    }
    
    if ((xattr_flags & SSH_FILEXFER_ATTR_EXTENDED) == SSH_FILEXFER_ATTR_EXTENDED) {
        // all extensions are stripped out
        attr_int32 = buffer_get_int(buff);
        *remaining_msg_len -= 4;
        msg_len -= 4;

        for (x = 1; x <= attr_int32; x++) {
            // extension name
            attr_string = buffer_get_string(buff, &attr_string_len);
            *remaining_msg_len -= (attr_string_len + 4);
            msg_len -= (attr_string_len + 4);

            // extension data
            attr_string = buffer_get_string(buff, &attr_string_len);
            *remaining_msg_len -= (attr_string_len + 4);
            msg_len -= (attr_string_len + 4);
        }
    }
    
    return 1;
}
