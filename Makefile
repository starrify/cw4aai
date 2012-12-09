# This source file is part of CW4AAI, CourseWork for Assembly and Interface
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

BIN = ./bin

all: $(BIN)/mass $(BIN)/memu $(BIN)/sys.img $(BIN)/app1.img $(BIN)/app2.img \
	$(BIN)/config.ini $(BIN)/aplay.img $(BIN)/smbtheme $(BIN)/earth.dat \
	$(BIN)/asciiart.img


$(BIN)/config.ini: memu/config.ini
	cp memu/config.ini $(BIN)/

$(BIN)/mass: mass/
	make -C mass/
	cp mass/mass $(BIN)

$(BIN)/memu: memu/ 
	make -C memu
	cp memu/memu $(BIN)

$(BIN)/preproc.pl: preproc/preproc.pl
	cp preproc/preproc.pl $(BIN)

$(BIN)/sys.img $(BIN)/app1.img $(BIN)/app2.img $(BIN)aplay.img $(BIN)/smbtheme \
$(BIN)/earth.dat $(BIN)/asciiart.img: os/ demo/ lib/ inc/ 
	make -C os
	make -C demo
	cp os/sys.img $(BIN)
	cp os/shell.img $(BIN)
	cp demo/app1.img $(BIN)
	cp demo/app2.img $(BIN)
	cp demo/aplay.img $(BIN)
	cp demo/smbtheme $(BIN)
	cp demo/earth.dat $(BIN)
	cp demo/asciiart.img $(BIN)

clean:
	make clean -C os
	make clean -C mass
	make clean -C memu

