.inc "pseudo_inst.inc"
.inc "var.inc"
.inc "sysinfo.inc"
.inc "jmp_fix.inc"
.inc "io.lib"
.inc "interrupt.lib"

.offset 0x1c00000 #28M

.def CUR_PROC_OFF {0}
.def PROC_INFO_OFF {4}
.static {
PROC:
CUR_PROC:
    .0
PROC_INFO:
    .inc "procinfo.b"
}

BOOT:
    #calc physical address for proc_info
    lli $t0, 0x1800000
    lla $t1, PROC_INFO
    sub $t1, $t1, $t0
    #set gdt position
    mtc0 $t1, $0
    #enable mmu
    mtc0 $zero, $4
    #move inst from real BT1 to virtual BT1
    #lla $t2, BT1
    #sub $t3, $t2, $t0
    #lw $t3, 0($t3)
    #sw $t3, 0($t2)
    
    j INIT
    
INIT:
    #init os stack
    lli $sp, SYS_STACK_INIT
    #init interrupt
    jal INT_INIT
    #init libs
    jal IO_INIT
    #interrupt enable
    ori $t0, $zero, 1
    mtc0 $t0, $3

TEST:
    ori $a0, $zero, 34
    and $a1, $zero, $zero
    ori $k0, $zero, SC_GOTOXY
    #jal SYS_SYSCALL
LOOP:
    jal GETC
    #jal SYS_GETC
    #blt $v0, $zero, LOOP
    or $a0, $zero, $v0
    jal PUTC
    #ori $k0, $zero, SC_PUTC
    #jal SYS_SYSCALL
    j LOOP

