C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall
CC=gcc

all: libcbrain.so

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -fPIC

libcbrain.so: ${OBJ}
	$(CC) -shared -o $@ -lpthread $(CFLAGS) $^

clean:
	rm src/*.o
	rm libcbrain.so
install: libcbrain.so
	install -m 777 $^ ${PREFIX}/lib/
	install -m 777 src/cbrain.h ${PREFIX}/include

uninstall:
	rm ${PREFIX}/lib/libcbrain.so
	rm ${PREFIX}/include/cbrain.h
