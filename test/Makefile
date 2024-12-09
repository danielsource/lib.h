.POSIX:
.SILENT:
.SUFFIXES:
.SUFFIXES: .c .o
.PHONY: clean test-run

include config.mk

objs = test.o

test: $(objs)
	@echo "  LD      $@"
	$(CC) -o $@ $(objs) $(CFLAGS) $(LDFLAGS)
test.o: test.c ../lib.h

clean:
	$(RM) *.o test
test-run: test
	./test

.c.o:
	@echo "  CC      $@"
	$(CC) -c $< -o $@ $(CFLAGS)
