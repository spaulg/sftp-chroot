#include "header/chroot_packets.h"

int chroot_packets(Buffer *buff, Buffer *preped_buff) {
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
	    case SSH_FXP_NAME:
		verbose("Rewriting SSH_FXP_NAME packet");
		packet_fxp_name(buff, preped_buff);
		break;
	    
	    case SSH_FXP_VERSION:
		verbose("Rewriting SSH_FXP_VERSION packet");
		packet_fxp_version(buff, preped_buff);
		break;

	    case SSH_FXP_ATTRS:
		verbose("Rewriting SSH_FXP_ATTRS packet");
		packet_fxp_attrs(buff, preped_buff);
		break;
	    
	    case SSH_FXP_STATUS:
	    case SSH_FXP_HANDLE:
	    case SSH_FXP_DATA:
	    case SSH_FXP_EXTENDED_REPLY:
		buffer_append(preped_buff, buffer_ptr(buff), msg_len + sizeof(u_int));
		buffer_consume(buff, msg_len + sizeof(u_int));
		break;
		
	    default:
		error("Unknown message %d", type);
		break;
	}
    }
   
    return 1;
}

int packet_fxp_version(Buffer *buff, Buffer *preped_buff) {
    u_char *cp;
    u_int msg_len;
    u_int xmsg_len;
    
    cp = buffer_ptr(buff);
    
    // 1. Ensure  extensions are not forwarded. Drop packet size down to 5 bytes + 4 byte length. Later byte are ignored through buffer_consume.
    msg_len = get_u32(cp);
    xmsg_len = 5;
    put_u32(cp, xmsg_len);

    // Capture server version
    protocol_ver = get_u32(cp+5);
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len + sizeof(u_int));
    buffer_consume(buff, msg_len + sizeof(u_int));
    
    return 1;
}

int packet_fxp_name(Buffer *buff, Buffer *preped_buff) {
    // Message
    u_int msg_len, xmsg_len;
    
    // File names
    u_int file_len, old_file_len, count, i;
    u_char *filename;
    
    // 1. Rewrite absolute filename responses to /. Ensure the packet length is updated.
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    count = get_u32(buffer_ptr(buff) + 9);
    
    // Copy first part of packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), 13);
    buffer_consume(buff, 13);
    xmsg_len -= 9;
    
    for (i = 1; i <= count; i++) {
	filename = buffer_get_string_ret(buff, &file_len);
	old_file_len = file_len;
	xmsg_len -= (file_len + 4);
	
	// Chroot filename, if applicable, and remove the length difference from the xmsg length
	filename = chroot_filename(filename, (u_char*) user_homedir);
	file_len = strlen((char*) filename);
	msg_len -= (old_file_len - file_len);
	
	// Copy string into preped buffer
	buffer_put_string(preped_buff, filename, file_len);
	
	// TODO: process long name - need to chroot the filename within the string - WinSCP uses the string for user/group names so it cannot be emptied
	filename = buffer_get_string_ret(buff, &file_len);
	buffer_put_string(preped_buff, filename, file_len);
	xmsg_len -= (file_len + 4);
	
	parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    }
    
    // Copy any remaining packet data over
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Rewrite message length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

int packet_fxp_attrs(Buffer *buff, Buffer *preped_buff) {
    u_char *cp;
    u_int msg_len, xmsg_len;
    
    cp = buffer_ptr(buff);
    
    // Copy first part of packet over to prepared buffer
    msg_len = get_u32(buffer_ptr(buff));
    xmsg_len = msg_len;
    
    buffer_append(preped_buff, buffer_ptr(buff), 9);
    buffer_consume(buff, 9);
    xmsg_len -= 5;
    
    // Clean attributes
    parse_attrs(buff, preped_buff, &msg_len, &xmsg_len);
    
    // Copy packet over to prepared buffer
    buffer_append(preped_buff, buffer_ptr(buff), xmsg_len);
    buffer_consume(buff, xmsg_len);
    
    // Replace length with new length
    put_u32(buffer_end(preped_buff)-msg_len-4, msg_len);
    
    return 1;
}

u_char *chroot_filename(u_char *filename, const u_char *chrootdir) {
    u_char *rewrite_filename;
    int file_len;
    int chrootdir_len = strlen((char*) chrootdir);
    
    debug("%s: Request to chroot filename: %s", __FUNCTION__, filename);
    if ((char) *filename == '/' && (void*) strstr((char*) filename, (char*) chrootdir) == (void*) filename) {
	// Filename is absolute and matches chroot directory
	debug("%s: Path matches chroot path (%s), chrooting to /", __FUNCTION__, chrootdir);
	
	// Allocate space for new filename
	file_len = strlen((char*) filename);
	file_len -= chrootdir_len;
	file_len++;				/* for the '/' */
	
	rewrite_filename = xmalloc(file_len + 1);  /* for the null byte */
	
	// Copy string portion without chroot directory
	strlcpy((char*) rewrite_filename, "/", 2);
	strncat((char*) rewrite_filename, (char*) filename+chrootdir_len, file_len-1);
	
	// Replace original with new
	xfree(filename);
	filename = rewrite_filename;
    }
    
    return filename;
}
