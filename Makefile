C_SOURCES = $(wildcard src/*.c)
HEADERS   = $(wildcard src/*.h)
OBJ = ${C_SOURCES:.c=.o}

all: brain

%.o: %.c
	gcc -Wall -c $< -o $@ -lpthread

brain: ${OBJ}
	gcc -Wall $^ -o $@ -lpthread

clean:
	rm brain
	rm src/*.o
