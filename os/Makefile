all: sys.img shell.img 

sys.t: sys.s *.lib *.inc preproc.pl
	./preproc.pl <sys.s >sys.t
sys.img: mass sys.t
	./mass sys.t -o sys.img
shell.t: shell.s *.lib *.inc preproc.pl
	./preproc.pl <shell.s >shell.t
shell.img: mass shell.t
	./mass shell.t -o shell.img
clean:	
	rm -f sys.t shell.t sys.img shell.img mass memu 1.log
