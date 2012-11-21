all: sys.img 

test.t: *.s *.lib *.inc test.s preproc.pl
	./preproc.pl < test.s > test.t
sys.img:mass test.t
	./mass test.t -o sys.img

clean:	
	rm -f test.t sys.img mass memu 1.log
