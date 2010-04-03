AM_CFLAGS = -I$(top_srcdir) -Wall

bin_PROGRAMS = sftp-chroot
sftp_chroot_SOURCES = main.c \
                      chroot_packets.c \
                      unchroot_packets.c \
                      packet_parser.c \
		      buffer.c \
		      log.c \
		      xmalloc.c \
		      vis.c \
		      fatal.c \
		      cleanup.c \
		      strlcpy.c \
		      bufaux.c \
		      misc.c \
		      bsd-asprintf.c

sftp_chroot_LDFLAGS = -lpthread $(AM_LDFLAGS)
