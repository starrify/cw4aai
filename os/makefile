sys.img: test.s
	./preproc.pl < test.s > test.t
	./mass test.t -o sys.img
clean:	
	rm test.t sys.img
