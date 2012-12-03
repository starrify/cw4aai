# This source file is part of CW4AAI, CourseWork for Assembly and Interface
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

BIN = ./bin

all: $(BIN)/mass $(BIN)/memu $(BIN)/sys.img $(BIN)/app1.img $(BIN)/app2.img

$(BIN)/mass: mass/
	make -C mass/
	cp mass/mass $(BIN)

$(BIN)/memu: memu/ 
	make -C memu
	cp memu/memu $(BIN)

$(BIN)/preproc.pl: os/preproc.pl
	cp os/preproc.pl $(BIN)

$(BIN)/sys.img $(BIN)/app1.img $(BIN)/app2.img: os/ 
	make -C os
	cp os/sys.img $(BIN)
	cp os/app1.img $(BIN)
	cp os/app2.img $(BIN)

clean:
	make clean -C os
	make clean -C mass
	make clean -C memu

