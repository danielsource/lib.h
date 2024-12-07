.POSIX:
.SILENT:
.SUFFIXES:
.SUFFIXES: .c .o
.PHONY: clean

CC = gcc
RM ?= rm -f
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wshadow -ggdb -O0 -DDEBUG -fanalyzer -fsanitize=address,leak,undefined
LDFLAGS =

objs = example.o

example: $(objs)
	@echo "  LD      $@"
	$(CC) -o $@ $(objs) $(CFLAGS) $(LDFLAGS)
example.o: example.c lib.h

clean:
	$(RM) *.o example

.c.o:
	@echo "  CC      $@"
	$(CC) -c $< -o $@ $(CFLAGS)
