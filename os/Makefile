BIN = ../bin

all: sys.img 

$(BIN)/preproc.pl: preproc.pl
	make bin/preproc.pl -C ..

$(BIN)/mass: ../mass
	make $(BIN)/mass -C ..

sys.t: sys.s *.lib *.inc $(BIN)/preproc.pl
	$(BIN)/preproc.pl <sys.s >sys.t

sys.img: $(BIN)/mass sys.t
	$(BIN)/mass sys.t -o sys.img

shell.t: shell.s *.lib *.inc $(BIN)/preproc.pl
	$(BIN)/preproc.pl <shell.s >shell.t

shell.img: $(BIN)mass shell.t
	$(BIN)/mass shell.t -o shell.img
clean:	
	rm -f sys.t shell.t sys.img shell.img mass memu 
