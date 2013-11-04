CC=cc
CFLAGS+=-std=c99 -I include -g

OBJS+=src/lex.o src/parse.o src/eval.o src/main.o

default: calc

calc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

%.c: %.yy Makefile include/**/*.h
	flex -o $@ $<
