CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

all: scramble

scramble: scramble.c turns.c
	gcc $(CFLAGS) scramble.c turns.c -o scramble.out $(LDFLAGS)