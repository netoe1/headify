PROGRAM_NAME := headify
CC ?= cc
CFLAGS ?= -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic

.PHONY: all clean

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): main.c parser.h
	$(CC) $(CFLAGS) -o $@ main.c

clean:
	rm -f $(PROGRAM_NAME) main
