.inc "jmp_fix.inc"
.inc "pseudo_inst.inc"
.inc "var.inc"
.inc "sysinfo.inc"
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
    #when running init code, treat system as an ordinary process
    lli $sp, PROC_USRSPACE_SIZE
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
    beq $v0, $zero, TEST1
#    ori $a0, $zero, 2
#    ori $k0, $zero, SC_FORK
#    syscall
LOOP:
    #set proc status as suspended
    #SETFRMLB PROC_INFO
    #ori $t0, $zero, PROC_SUSPEND
    #sw $t0, PROC_STATE_OFF_PER_PROC($fp)
    #wait
    ori $a0, $zero, 98
    jal PUTC
    wait
    j LOOP
TEST1:
    ori $a0, $zero, 97
    jal PUTC
    j TEST
    #jal GETC
    #jal SYS_GETC
    #blt $v0, $zero, LOOP
    #or $a0, $zero, $v0
    #jal PUTC
    #ori $k0, $zero, SC_PUTC
    #jal SYS_SYSCALL
END:
    #ori $a0, $zero, 99
    #jal SYS_GETC
    j END

