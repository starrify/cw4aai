# This source file is part of MASS: Mips ASSembler 
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

CC = gcc
CFLAG = -O2

# override the buildin rules by GNU make
%.c: %.y
%.c: %.l

mass: mass.tab.o mass.yy.o mass.o bingen.o symbol.o
	$(CC) $(CFLAG) mass.tab.o mass.yy.o mass.o bingen.o symbol.o -o mass
	
mass.tab.o: mass.tab.c mass.h
	$(CC) $(CFLAG) -c mass.tab.c -o mass.tab.o

mass.tab.c mass.tab.h: mass.y mass.h bingen.h symbol.h
	bison -d -o mass.tab.c mass.y    
	
mass.yy.o: mass.yy.c mass.tab.h mass.h
	$(CC) $(CFLAG) -c mass.yy.c -o mass.yy.o
	
mass.yy.c mass.yy.h: mass.lex mass.tab.h mass.h
	flex -i --header-file=mass.yy.h -o mass.yy.c mass.lex
	
mass.o: mass.c mass.h mass.yy.h mass.tab.h
	$(CC) $(CFLAG) -c mass.c -o mass.o
	
bingen.o: bingen.c mass.h mass.tab.h symbol.h
	$(CC) $(CFLAG) -c bingen.c -o bingen.o

symbol.o: symbol.c symbol.h mass.h mass.tab.h
	$(CC) $(CFLAG) -c symbol.c -o symbol.o
	
wc:
	wc mass.y mass.lex mass.c mass.h bingen.c bingen.h symbol.c symbol.h

clean:
	rm -f mass.tab.c mass.tab.h mass.tab.o mass.yy.c mass.yy.h mass.yy.o \
		mass.o bingen.o symbol.o mass *~

