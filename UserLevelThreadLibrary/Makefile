CC = gcc
CFLAGS = -Wall -pthread -Wno-unused-variable

SRC = main.c	scheduler.c	threads.c
HDR = bank.h	threadtools.h
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: main

main: $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o main

%.o: %.c $(HDR)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ) main
