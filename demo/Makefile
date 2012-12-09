BIN = ../bin
EXT_DEP = ../inc/* ../lib/*
all: app1.img app2.img aplay.img

$(BIN)/preproc.pl: ../preproc/preproc.pl
	make bin/preproc.pl -C ..

$(BIN)/mass: ../mass
	make bin/mass -C ..

app1.t: app1.s $(EXT_DEP) $(BIN)/preproc.pl
	$(BIN)/preproc.pl <app1.s >app1.t

app1.img: $(BIN)/mass app1.t
	$(BIN)/mass app1.t -o app1.img

app2.t: app2.s $(EXT_DEP) $(BIN)/preproc.pl
	$(BIN)/preproc.pl <app2.s >app2.t

app2.img: $(BIN)/mass app2.t
	$(BIN)/mass app2.t -o app2.img

aplay.t: aplay.s $(EXT_DEP) $(BIN)/preproc.pl
	$(BIN)/preproc.pl < aplay.s >aplay.t

aplay.img: $(BIN)/mass aplay.t
	$(BIN)/mass aplay.t -o aplay.img


clean:	
	rm -f app1.t app2.t app1.img app2.img aplay.t aplay.img
