CC=gcc
CFLAGS=
LIBS=-lm

all: build/main

.PHONY: objects
objects: $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@

build/main: objects
	mkdir -p build
	${CC} build/*.o ${LIBS} -o $@

.PHONY: clean
clean:
	rm -rf build
