# This source file is part of CW4AAI, CourseWork for Assembly and Interface
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

all: mass/mass memu/memu os/sys.img
	

mass/mass: mass/
	make -C mass/

memu/memu: memu/ os/sys.img
	cp os/sys.img memu/
	make -C memu
	# for zlk..
	cp memu/memu os/

os/sys.img: os/ mass/mass
	cp mass/mass os/
	make -C os

clean:
	make clean -C os
	make clean -C mass
	make clean -C memu

