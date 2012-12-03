.inc "../inc/jmp_fix.inc"
.inc "../inc/pseudo_inst.inc"
.inc "../inc/var.inc"
.inc "../lib/io.lib"
.inc "interrupt.lib"
.inc "sysinfo.inc"

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
    
    j SYS_INIT
    
SYS_INIT:
    #init os stack
    #when running init code, treat system as an ordinary process
    lli $sp, PROC_USRSPACE_SIZE
    #init interrupt
    jal INT_INIT
    #init libs
    #jal IO_INIT
    #interrupt enable
 
    mtc0 $t0, $3

MAIN:
    ori $a0, $zero, 1
    ori $k0, $zero, SC_FORK
    syscall
    beq $v0, $zero, CHILD
FATHER:
    #set proc status as suspended
    SETFRMLB PROC_INFO
    ori $t0, $zero, PROC_SUSPEND
    sw $t0, PROC_STATE_OFF_PER_PROC($fp)
    wait
    j ENDFATH

CHILD:
    ori $a0, $zero, 1
    ori $k0, $zero, SC_EXEC
    syscall
    #ori $a0, $zero, 97
    #jal PUTC
    #j ENDCHLD

ENDCHLD:
    j ENDCHLD
ENDFATH:
    j ENDFATH

