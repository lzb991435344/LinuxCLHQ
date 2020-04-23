CFLAGS+=-I../include/ -pthread -g
LDFLAGS+=-pthread
#-I /home/Project/LinuxLHQ/ipv4mulbrocast/src/include/
all:servertest

servertest:server.o thr_channel.o thr_list.o mytbf.o medialib.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)


