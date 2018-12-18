C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: brain

%.o: %.c
	gcc -Wall -c $< -o $@

brain: ${OBJ}
	gcc -Wall $^ -o $@

clean:
	rm brain
	rm src/*.o
