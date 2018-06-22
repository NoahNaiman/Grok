#Makefile to build and compile Grok text editor

CC = gcc
CFLAGS = -c -Wall
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bin/piece_chain

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
