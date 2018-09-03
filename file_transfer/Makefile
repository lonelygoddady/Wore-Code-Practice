#!/bin/sh

CC= gcc
RM=rm
CCFLAGS= 
UTILITY = utility
TARGET_C = client
TARGET_S = server

default: $(TARGET_C).o $(TARGET_S).o $(UTILITY).o
	$(CC) $(CCFLAGS) -o $(TARGET_C) $(TARGET_C).o $(UTILITY).o
	$(CC) $(CCFLAGS) -o $(TARGET_S) $(TARGET_S).o $(UTILITY).o

debug:  CCFLAGS = -DDEBUG
debug:	default

$(UTILITY).o: $(UTILITY).c
	$(CC) $(CCFLAGS) -c $(UTILITY).c 

$(TARGET_C).o: $(TARGET_C).c
	$(CC) $(CCFLAGS) -c $(TARGET_C).c

$(TARGET_S).o: $(TARGET_S).c
	$(CC) $(CCFLAGS) -c $(TARGET_S).c

all: default 

clean:
	$(RM) $(TARGET_C) $(TARGET_S) $(TARGET_C).o $(TARGET_S).o $(UTILITY).o cli_rec server_rec
