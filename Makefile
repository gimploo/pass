CC = gcc
CCFLAGS = -W -Wall -Wextra -pedantic -g

all: pass

debug: CCFLAGS += -DDEBUG
debug: pass

pass: 
	$(CC) $(CCFLAGS) src/pass.c -o build/pass

clean:
	rm -rf build/*
