#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

CC = gcc

CFLAGS = -Wall

INCLUDES = -Iinclude

LFLAGS = -L./lib

LIBS =

SRCS = main.c

OBJS = $(SRCS:.c=.o)

MAIN = monitor

.PHONY: depend clean

all:	$(MAIN)
	@echo Simple monitor has been compiled

$(MAIN):	$(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend:	$(SRCS)
	makedepend $(INCLUDES) $^

debug: CFLAGS += -DDEBUG -g3
debug: $(MAIN)

# DO NOT DELETE THIS LINE -- make depend needs it

main.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
main.o: /usr/include/features.h /usr/include/stdc-predef.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/bits/types/__FILE.h /usr/include/bits/types/FILE.h
main.o: /usr/include/libio.h /usr/include/_G_config.h
main.o: /usr/include/bits/types/__mbstate_t.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
main.o: /usr/include/bits/floatn.h /usr/include/sys/types.h
main.o: /usr/include/bits/types/clock_t.h /usr/include/bits/types/clockid_t.h
main.o: /usr/include/bits/types/time_t.h /usr/include/bits/types/timer_t.h
main.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/bits/byteswap-16.h /usr/include/bits/uintn-identity.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/types/sigset_t.h
main.o: /usr/include/bits/types/__sigset_t.h
main.o: /usr/include/bits/types/struct_timeval.h
main.o: /usr/include/bits/types/struct_timespec.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/sysmacros.h
main.o: /usr/include/bits/pthreadtypes.h
main.o: /usr/include/bits/thread-shared-types.h
main.o: /usr/include/bits/pthreadtypes-arch.h /usr/include/alloca.h
main.o: /usr/include/bits/stdlib-float.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/environments.h
main.o: /usr/include/bits/confname.h /usr/include/bits/getopt_posix.h
main.o: /usr/include/bits/getopt_core.h /usr/include/sys/socket.h
main.o: /usr/include/bits/types/struct_iovec.h /usr/include/bits/socket.h
main.o: /usr/include/bits/socket_type.h /usr/include/bits/sockaddr.h
main.o: /usr/include/asm/socket.h /usr/include/asm-generic/socket.h
main.o: /usr/include/asm/sockios.h /usr/include/asm-generic/sockios.h
main.o: /usr/include/bits/types/struct_osockaddr.h /usr/include/arpa/inet.h
main.o: /usr/include/netinet/in.h /usr/include/bits/stdint-uintn.h
main.o: /usr/include/bits/in.h /usr/include/netinet/if_ether.h
main.o: /usr/include/linux/if_ether.h /usr/include/linux/types.h
main.o: /usr/include/asm/types.h /usr/include/asm-generic/types.h
main.o: /usr/include/asm-generic/int-ll64.h /usr/include/asm/bitsperlong.h
main.o: /usr/include/asm-generic/bitsperlong.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
main.o: /usr/include/asm/posix_types.h /usr/include/asm/posix_types_64.h
main.o: /usr/include/asm-generic/posix_types.h /usr/include/net/ethernet.h
main.o: /usr/include/stdint.h /usr/include/bits/wchar.h
main.o: /usr/include/net/if_arp.h /usr/include/netinet/ip.h include/test.h
