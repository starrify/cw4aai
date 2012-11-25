all: sys.img 

sys.t: *.s *.lib *.inc sys.s preproc.pl
	./preproc.pl < sys.s > sys.t
sys.img:mass sys.t
	./mass sys.t -o sys.img

clean:	
	rm -f sys.t sys.img mass memu 1.log
