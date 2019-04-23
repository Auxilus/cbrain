C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall
CC=gcc

all: cbrain

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

cbrain: ${OBJ}
	$(CC) $^ -o $@ -lpthread $(CFLAGS)
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $@

clean:
	rm cbrain
	rm src/*.o
