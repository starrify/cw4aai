.inc "pseudo_inst.inc"
.inc "var.inc"
.inc "sysinfo.inc"
.inc "jmp_fix.inc"
.inc "io.lib"
.inc "interrupt.lib"

.offset 0x1c00000 #28M

BOOT:
    #calc physical address for proc_info
    lli $t0, 0x1800000
    lla $t1, PROC_INFO
    sub $t1, $t1, $t0
    #set gdt position
    mtc0 $t1, $0
    #enable mmu
    mtc0 $zero, $4
    
    j INIT
    
INIT:
    #init os stack
    lli $sp, SYS_STACK_INIT
    #init interrupt
    jal INT_INIT
    #init libs
    jal IO_INIT
    #interrupt enable
 
    mtc0 $t0, $3

TEST:
    #jal SYS_SYSCALL
    ori $a0, $zero, 1
    ori $k0, $zero, SC_FORK
    syscall
    #ori $a0, $zero, 2
    #ori $k0, $zero, SC_FORK
    #syscall
LOOP:
    #set proc status as suspended
    SETFRMLB PROC_INFO
    ori $t0, $zero, PROC_SUSPEND
    #NOTE!!!: gdt size may be changed in the future
    sw $t0, 172($fp)
    #jal GETC
    #jal SYS_GETC
    #blt $v0, $zero, LOOP
    #or $a0, $zero, $v0
    #jal PUTC
    #ori $k0, $zero, SC_PUTC
    #jal SYS_SYSCALL
    j LOOP

