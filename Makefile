C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: cbrain

%.o: %.c
	gcc -Wall -c $< -o $@

cbrain: ${OBJ}
	gcc -Wall $^ -o $@ -lpthread -lSDL2

clean:
	rm cbrain
	rm src/*.o
