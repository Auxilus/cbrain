C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}
CFLAGS = -Wall -s -m32 -fno-stack-protector -fsingle-precision-constant -fno-unwind-tables -fno-asynchronous-unwind-tables -ffast-math -ffunction-sections -fdata-sections

all: cbrain

%.o: %.c
	gcc -c $< -o $@ $(CFLAGS)

cbrain: ${OBJ}
	gcc $^ -o $@ -lpthread $(CFLAGS)
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $@

clean:
	rm cbrain
	rm src/*.o
	rm brain.txt
