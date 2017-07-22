# Filename: Makefile
# AUthor: Johannes Pikel
# Date: 2017.07.17
# ONID: pikelj
# Class: CS362-400
# Assignment: Random Testing Quiz 2
# Description: makefile to compile random testing quiz 2
#

CC = gcc
CFLAGS = -x c 
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -pedantic-errors

SRCS = testme.c

all: ${SRCS}
	${CC} ${CLAGS} ${SRCS} -o testme
	./testme

clean:
	rm -f *.o testme
