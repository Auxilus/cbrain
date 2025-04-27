C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall -O3 -fcommon
CC=gcc

all: libcbrain.so

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -fPIC

libcbrain.so: ${OBJ}
	$(CC) -shared -o $@ -lSDL2 -lpthread $(CFLAGS) $^
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $@

clean:
	rm -f src/*.o
	rm -f examples/libcbrain.so
	rm -f examples/tem_test
	rm -f examples/cbrain

install: libcbrain.so
	install -m 777 $^ ${PREFIX}/lib/
	install -m 777 src/cbrain.h ${PREFIX}/include

uninstall:
	rm ${PREFIX}/lib/libcbrain.so
	rm ${PREFIX}/include/cbrain.h
