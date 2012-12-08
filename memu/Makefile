# This source file is part of MEMU: Mips EMUlator
# by Pengyu CHEN(cpy.prefers.you@gmail.com)
# COPYLEFT, ALL WRONGS RESERVED.
 
# oops this makefile sucks..

CC  = gcc
CFLAG   = -O2 
LFLAG   = -lpthread -lncurses -liniparser
OBJS    = memu.o config.o decode.o inst.o mem.o mmu.o reg.o \
            daemon.o fancyterm.o interrupt.o hdd.o
SOURCES = memu.c config.c decode.c inst.c mem.c mmu.c reg.c \
			daemon.c fancyterm.c  interrupt.c hdd.c\
		  memu.h config.h decode.h exception.h inst.h mem.h mmu.h reg.h \
			daemon.h fancyterm.h interrupt.h hdd.h

all: memu 

memu: $(OBJS)
	$(CC) $(CFLAG) $(LFLAG) $(OBJS) -o memu

memu.o: memu.c memu.h config.h decode.h exception.h mem.h mmu.h reg.h \
        daemon.h fancyterm.h interrupt.h hdd.h
	$(CC) $(CFLAG) -c memu.c -o memu.o

config.o: config.c config.h
	$(CC) $(CFLAG) -c config.c -o config.o

decode.o: decode.c decode.h memu.h config.h
	$(CC) $(CFLAG) -c decode.c -o decode.o
	
#exception.o: exception.c exception.h
#	$(CC) $(CFLAG) -c exception.c -o exception.o
	
inst.o: inst.c inst.h config.h exception.h memu.h decode.h reg.h mem.h mmu.h interrupt.h
	$(CC) $(CFLAG) -c inst.c -o inst.o

mem.o: mem.c mem.h config.h exception.h memu.h
	$(CC) $(CFLAG) -c mem.c -o mem.o

mmu.o: mmu.c mmu.h exception.h memu.h mem.h config.h reg.h
	$(CC) $(CFLAG) -c mmu.c -o mmu.o

reg.o: reg.c reg.h config.h memu.h
	$(CC) $(CFLAG) -c reg.c -o reg.o

daemon.o: daemon.c daemon.h config.h mem.h reg.h interrupt.h
	$(CC) $(CFLAG) -c daemon.c -o daemon.o

interrupt.o: interrupt.c interrupt.h exception.h reg.h config.h
	$(CC) $(CFLAG) -c interrupt.c -o interrupt.o

hdd.o: hdd.c hdd.h memu.h config.h
	$(CC) $(CFLAG) -c hdd.c -o hdd.o
	
gt64120.o: gt64120.c gt64120.h memu.h malta_mem.h
	$(CC) $(CFLAG) -c gt64120.c -o gt64120.o
	
iic.o: iic.c iic.h memu.h malta_mem.h config.h
	$(CC) $(CFLAG) -c iic.c -o iic.o

fancyterm.o: fancyterm.c fancyterm.h
	$(CC) $(CFLAG) -c fancyterm.c -o fancyterm.o
	
wc:
	wc $(SOURCES)

clean:
	rm -f $(OBJS) memu *~ .*.swp 

