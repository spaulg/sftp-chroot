
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

#include "header/unchroot_packets.h"

int unchroot_packets(Buffer *buff, Buffer *preped_buff) {
    u_int buf_len;
    u_int msg_len;
    u_int type;
    u_char *cp;
    
    // loop while there are still packets to be processed
    for (;;) {
	// Validate minimum buffer length requirement. Buffer must be at least 5 bytes long to create a message.
	buf_len = buffer_len(buff);
	if (buf_len < 5)
		return 0;		/* Incomplete message. */

	// find the length of this message and validate to maximum message length. Ensure the buffer offset does not move forward.
	cp = buffer_ptr(buff);
	
	msg_len = get_u32(cp);
	if (msg_len > SFTP_MAX_MSG_LENGTH) {
	    error("bad message from");
	    cleanup_exit(11);
	}
	
	// Validate the entire message is available for processing
	if (buf_len < msg_len + 4)
	    return 0;
	
	// Compensate for message length field
	cp += sizeof(u_int);
	
	type = get_u8(cp);
	switch (type) {
	    case SSH_FXP_INIT:
		verbose("Rewriting SSH_FXP_INIT packet");
		packet_fxp_init(buff, preped_buff);
		break;
	    
	    case SSH_FXP_OPEN:
		verbose("Rewriting SSH_FXP_OPEN packet");
		packet_fxp_open(buff, preped_buff);
		break;
	    
	    case SSH_FXP_LSTAT:
		verbose("Rewriting SSH_FXP_LSTAT packet");
		packet_fxp_basicpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_SETSTAT:
		verbose("Rewriting SSH_FXP_SETSTAT packet");
		packet_fxp_setstat(buff, preped_buff);
		break;
	    
	    case SSH_FXP_FSETSTAT:
		verbose("Rewriting SSH_FXP_SETSTAT packet");
		packet_fxp_fsetstat(buff, preped_buff);
		break;
	    
	    case SSH_FXP_OPENDIR:
		verbose("Rewriting SSH_FXP_OPENDIR packet");
		packet_fxp_basicpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_REMOVE:
		verbose("Rewriting SSH_FXP_REMOVE packet");
		packet_fxp_basicpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_MKDIR:
		verbose("Rewriting SSH_FXP_MKDIR packet");
		packet_fxp_mkdir(buff, preped_buff);
		break;
	    
	    case SSH_FXP_RMDIR:
		verbose("Rewriting SSH_FXP_RMDIR packet");
		packet_fxp_basicpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_REALPATH:
		verbose("Rewriting SSH_FXP_REALPATH packet");
		packet_fxp_realpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_STAT:
		verbose("Rewriting SSH_FXP_STAT packet");
		packet_fxp_basicpath(buff, preped_buff);
		break;
	    
	    case SSH_FXP_RENAME:
		verbose("Rewriting SSH_FXP_RENAME packet");
		packet_fxp_rename(buff, preped_buff);
		break;
	    
	    case SSH_FXP_READLINK:
		verbose("Rewriting SSH_FXP_READLINK packet");
		packet_fxp_readlink(buff, preped_buff);
		break;
	    
	    case SSH_FXP_SYMLINK:
		verbose("Rewriting SSH_FXP_SYMLINK packet");
		packet_fxp_symlink(buff, preped_buff);
		break;
	    
	    case SSH_FXP_EXTENDED:
		verbose("Rewriting SSH_FXP_EXTENDED packet");
		packet_fxp_extension(buff, preped_buff);
		break;
	    
	    case SSH_FXP_CLOSE:
	    case SSH_FXP_READDIR:
	    case SSH_FXP_FSTAT:
	    case SSH_FXP_READ:
	    case SSH_FXP_WRITE:
		// These packets pass straight through without modification
		buffer_append(preped_buff, buffer_ptr(buff), msg_len + sizeof(u_int));
		buffer_consume(buff, msg_len + sizeof(u_int));
		break;
	    
	    default:
		fatal("Unknown message %d", type);
		break;
	}
    }

    return 1;
}

int packet_fxp_init(Buffer *buff, Buffer *preped_buff) {
    u_char *cp;
    u_int msg_len;
    u_int xmsg_len;
    u_int msg_ver;
    
    cp = buffer_ptr(buff);
    
    // 1. Ensure  extensions are not forwarded. Drop packet size down to 5 bytes + 4 byte length. Later byte are ignored through buffer_consume.
    msg_len = get_u32(cp);
    xmsg_len = 5;
    put_u32(cp, xmsg_len);
    
    // 2. Ensure client version number presented is not greater than that supported.
    cp += 5;
    msg_ver = get_u32(cp);
    if (msg_ver > 3)
	msg_ver = 3;
    
    put_u32(cp, msg_ver);
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len + sizeof(u_int));
    buffer_consume(buff, msg_len + sizeof(u_int));
    
    return 1;
}

int packet_fxp_basicpath(Buffer *buff, Buffer *preped_buff) {
    u_char *cp;
    u_int msg_len, xmsg_len;
    u_int file_len;
    u_char *filename;
    
    cp = buffer_ptr(buff);
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Replace length with new length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_open(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len, pflags;
    u_char *filename;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // pflags
    pflags = buffer_get_int(buff);
    buffer_put_int(preped_buff, pflags);
    xmsg_len -= 4;
    
    // Copy attributes through, cleaning extensions where required
    parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_setstat(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len;
    u_char *filename;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy attributes through, cleaning extensions where required
    parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_fsetstat(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len;
    u_char *handle;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Copy handle
    handle = buffer_get_string(buff, &file_len);
    buffer_put_string(preped_buff, (char*) handle, file_len);
    xmsg_len -= (file_len + 4);
    
    // Copy attributes through, cleaning extensions where required
    parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_mkdir(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len;
    u_char *filename;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy attributes through, cleaning extensions where required
    parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_realpath(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len, xmsg_len;
    u_int file_len;
    u_char *filename;
      
     // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Replace length with new length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_rename(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len;
    u_char *filename;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite current filename
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Rewrite new filename
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_readlink(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
       
    // 1. Rewrite the filename requested to the users home directory. Ensure the packet length is updated.
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len + sizeof(u_int));
    buffer_consume(buff, msg_len + sizeof(u_int));
    
    return 1;
}

int packet_fxp_symlink(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // File names
    u_int file_len;
    u_char *filename;
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Rewrite link path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Rewrite target path
    filename = buffer_get_string(buff, &file_len);
    filename = unchroot_filename(filename, (u_char*) user_homedir);
    buffer_put_cstring(preped_buff, (char*) filename);
    xmsg_len -= (file_len + 4);
    msg_len += (strlen((char*) filename) - file_len);
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_extension(Buffer *buff, Buffer *preped_buff) {
    u_int msg_len;
    u_int xmsg_len;
    
    // 1. Rewrite the request type to 0, destroying the packet
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len + sizeof(u_int));
    buffer_consume(buff, msg_len + sizeof(u_int));
    
    return 1;
}

u_char *unchroot_filename(u_char *filename, const u_char *chrootdir) {
    // Chroot filename. Relative paths default to the home directory anyway, so these may be automatically
    // chrooted to the users home directory. Absolite paths are concatenated with the home directory, as they are assumed to
    // already by chrooted (and if they're not, it is an attempt to break out the chroot).
    u_char *rewrite_filename, *translated_path;
    int file_len;
    int chrootdir_len = strlen((char*) chrootdir);
    int maxpathlen;
    
    debug("%s: Request to unchroot path: %s", __FUNCTION__, filename);
    // Allocate space for new filename
    file_len = strlen((char*) filename);
    file_len += chrootdir_len;
    
    if ((char) *filename != '/')
	file_len++;	/* additional slash */
    
    rewrite_filename = xmalloc(file_len + 1);  /* for the null byte */
    
    // Chroot directory
    strlcpy((char*) rewrite_filename, (char*) chrootdir, chrootdir_len + 1);

    // Trailing slash for relative paths    
    if ((char) *filename != '/')
	strncat((char*) rewrite_filename, "/", 1); /* additional slash */

    // Actual path    
    strncat((char*) rewrite_filename, (char*) filename, (file_len - chrootdir_len));

    // Validate with realpath
    maxpathlen = pathconf("/", _PC_PATH_MAX);
    maxpathlen++;
    translated_path = xmalloc(maxpathlen + 1);
    
    realpath((char*) rewrite_filename, (char*) translated_path);
    free(rewrite_filename);
    
    // Verify translated path is within chroot path
    if ((void*) strstr((char*) translated_path, (char*) chrootdir) != (void*) translated_path) {
	// Path has fallen outside translated path, rewrite to home directory
	debug("%s: Path escapes chroot root path. Defaulting to chroot root path", __FUNCTION__);
	free(translated_path);
	translated_path = (u_char*) chrootdir;
    }
    
    // Replace original with new
    xfree(filename);
    filename = translated_path;
    debug("%s: Unchrooted path: %s", __FUNCTION__, filename);
    
    return filename;
}
