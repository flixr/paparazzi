# Hey Emacs, this is a -*- makefile -*-

CC = g++
CFLAGS = -Wall
LDFLAGS = -Wall

all: parser

parser: parser.o
	$(CC) $(LDLAGS) -o parser parser.o

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o parser *~
