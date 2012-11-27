all: sys.img app1.img app2.img 

sys.t: sys.s *.lib *.inc preproc.pl
	./preproc.pl <sys.s >sys.t
sys.img: mass sys.t
	./mass sys.t -o sys.img
shell.t: shell.s *.lib *.inc preproc.pl
	./preproc.pl <shell.s >shell.t
shell.img: mass shell.t
	./mass shell.t -o shell.img
app1.t: app1.s *.lib *.inc preproc.pl
	./preproc.pl <app1.s >app1.t
app1.img: mass app1.t
	./mass app1.t -o app1.img
app2.t: app2.s *.lib *.inc preproc.pl
	./preproc.pl <app2.s >app2.t
app2.img: mass app1.t
	./mass app2.t -o app2.img
clean:	
	rm -f sys.t shell.t app1.t app2.t sys.img shell.img app1.img app2.img mass memu 1.log
