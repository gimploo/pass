CC = gcc
CCFLAGS = -W -Wall -Wextra -pedantic

all: pass

debug: CCFLAGS += -g
debug: pass

pass: 
	$(CC) $(CCFLAGS) src/pass.c -o build/pass

clean:
	rm -rf build/*
