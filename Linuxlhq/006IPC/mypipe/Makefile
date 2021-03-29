CFLAGS+=-pthread
LDFLAGS+=-pthread 
all:mypipe 

mypipe:mypipe.o mypipe.o
	gcc $^ -o  $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o mypipe