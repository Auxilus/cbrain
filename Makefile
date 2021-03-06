C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall -O3
CC=gcc

all: libcbrain.so

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -fPIC

libcbrain.so: ${OBJ}
	$(CC) -shared -o $@ -lSDL2 -lpthread $(CFLAGS) $^
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $@

clean:
	rm src/*.o
	rm libcbrain.so
	rm examples/tem_test
	rm examples/cbrain

install: libcbrain.so
	install -m 777 $^ ${PREFIX}/lib/
	install -m 777 src/cbrain.h ${PREFIX}/include

uninstall:
	rm ${PREFIX}/lib/libcbrain.so
	rm ${PREFIX}/include/cbrain.h
