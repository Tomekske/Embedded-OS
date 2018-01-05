#Variablen declareren
CFILE=*.c
OBJNAME=blinky
CC=gcc
CFLAGS=-wall	#turns warning flaggs on
CVER=-std=c11
LIBNAME=bcm2835
OUT:=-o $(OBJNAME)
LIB:=-l $(LIBNAME)

all:
	$(CC) $(CVER) $($CFLAGS) $(CFILE) $(OUT) $(LIB)
clean:
	rm -rf *o $(OBJNAME)