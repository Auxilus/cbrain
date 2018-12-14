C_SOURCES = $(wildcard *.c)
HEADERS   = $(wildcard *.h)
OBJ = ${C_SOURCES:.c=.o}

all: brain

brain: brain.c ${HEADERS}
	gcc -Wall $< -o $@

clean:
	rm brain
