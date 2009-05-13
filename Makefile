flags = -Wall -std=c99

headers = $(wildcard *.h)
impls = $(wildcard *.c)

frameworks = -lGL -lGLEW -lSDL

all: shader-inc

shader-inc:
	gcc ${flags} -o $@ ${impls} ${frameworks}

.PHONY : clean

clean:
	rm -f shader-inc
