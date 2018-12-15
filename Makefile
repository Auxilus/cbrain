C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: brain

brain: src/brain.c ${HEADERS}
	gcc -Wall $< -o $@

clean:
	rm brain
