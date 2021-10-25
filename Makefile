CC = gcc
CCFLAGS = -W -Wall -Wextra -pedantic -g

all: pass

debug: CCFLAGS += -DDEBUG
debug: pass

pass: 
	$(CC) $(CCFLAGS) src/pass.c -o bin/pass

clean:
	rm -rf bin/*
