
#  Copyright 2013 Simon Paulger <spaulger@codezen.co.uk>
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

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
