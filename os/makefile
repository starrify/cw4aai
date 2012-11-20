sys.img: *.s *.lib *.inc
	./preproc.pl < test.s > test.t
	./mass test.t -o sys.img
clean:	
	rm test.t sys.img
