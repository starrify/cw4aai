BIN = ../bin

all: sys.img app1.img app2.img

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

app1.t: app1.s *.lib *.inc $(BIN)/preproc.pl
	$(BIN)/preproc.pl <app1.s >app1.t

app1.img: $(BIN)/mass app1.t
	$(BIN)/mass app1.t -o app1.img

app2.t: app2.s *.lib *.inc $(BIN)/preproc.pl
	$(BIN)/preproc.pl <app2.s >app2.t

app2.img: $(BIN)/mass app2.t
	$(BIN)/mass app2.t -o app2.img

clean:	
	rm -f sys.t shell.t sys.img shell.img app1.t app1.img app2.t app2.img
