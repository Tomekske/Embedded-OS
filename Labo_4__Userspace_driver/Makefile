#Variablen declareren
CFILE=$(notdir  ../$(wildcard *.c))	#get filename
OBJNAME=$(basename $(CFILE)) #get basename of file
CC=gcc
CFLAGS=-wall	#turns warning flaggs on
CVER=-std=c11
LIBNAME=bcm2835
OUT:=-o $(OBJNAME)
LIB:=-l $(LIBNAME)

all: compile run

compile:
	$(shell $(CC) $(CVER) $($CFLAGS) $(CFILE) $(OUT) $(LIB))
	@echo Compiled program $(CFILE)

run:
	@echo Running program $(OBJNAME)
	sudo ./$(OBJNAME)
clean:
	$(shell rm -rf *o $(OBJNAME))
	@echo Removed $(OBJNAME) 