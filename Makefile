# This source file is part of CW4AAI, CourseWork for Assembly and Interface
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

BIN = ./bin

all: $(BIN)/mass $(BIN)/memu $(BIN)/sys.img

$(BIN)/mass: mass/
	make -C mass/
	cp mass/mass $(BIN)

$(BIN)/memu: memu/ 
	make -C memu
	cp memu/memu $(BIN)

$(BIN)/preproc.pl: os/preproc.pl
	cp os/preproc.pl $(BIN)

$(BIN)/sys.img: os/ 
	make -C os
	cp os/sys.img $(BIN)

clean:
	make clean -C os
	make clean -C mass
	make clean -C memu

