#Variablen declareren
CFILE=$(notdir  ../$(wildcard *.c))	#get filename
OBJNAME=$(basename $(CFILE)) #get basename of file
CC=gcc
CFLAGS=-wall	#turns warning flaggs on
CVER=-std=c11
LIBNAME=bcm2835
OUT:=-o $(OBJNAME)
LIB:=-l $(LIBNAME)
ARG=-m
all: compile run

compile:
	$(shell $(CC) $(CVER) $($CFLAGS) $(CFILE) $(OUT) $(LIB))
	@echo Compiled program $(CFILE)

run: run_msg
	sudo ./$(OBJNAME) $(ARG)
	
run_msg:
	@echo Running program $(OBJNAME)
clean:
	$(shell rm -rf *o $(OBJNAME))
	@echo Removed $(OBJNAME) 