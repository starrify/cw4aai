.inc "pseudo_inst.inc"
.inc "var.inc"
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
    #move inst from real BT1 to virtual BT1
    lla $t2, BT1
    sub $t3, $t2, $t0
    lw $t3, 0($t3)
    sw $t3, 0($t2)
    j BT1
BT1:
    mtc0 $t1, $0
    
INIT:
    #init os stack
    lli $sp, OS_STACK_INIT
    jal INT_INIT
    #interrupt enable
    ori $t0, $zero, 1
    mtc0 $t0, $3

TEST:
    ori $a0, $zero, 34
    and $a1, $zero, $zero
    ori $k0, $zero, SC_GOTOXY
    jal SYS_SYSCALL
LOOP:
    ori $k0, $zero, SC_GETC
    jal SYS_SYSCALL
    blt $v0, $zero, LOOP
    or $a0, $zero, $v0
    jal PUTC
    #ori $k0, $zero, SC_PUTC
    #jal SYS_SYSCALL
    j LOOP
WTF:
    eret
#INIT:
    #addiu $t0, $zero, 1000
    #SYSINFO_S $t0, SCR_BASE
#    ori $sp, $zero, OS_STACK_INIT
#    ori $a0, $zero, 34
#    ori $a1, $zero, 78
#    jal SYS_GOTOXY
#    ori $a0, $zero, 104
#    jal SYS_PUTC
#    ori $a0, $zero, 101
#    jal SYS_PUTC
#    ori $a0, $zero, 108
#    jal SYS_PUTC
#    ori $a0, $zero, 108
#    jal SYS_PUTC
#    ori $a0, $zero, 111
#    jal SYS_PUTC
#    ori $a0, $zero, 34
#    ori $a1, $zero, 78
#    jal SYS_GOTOXY
#TEST:
#    ori $a0, $zero, 104
#    jal SYS_PUTC
#    ori $a0, $zero, 101
#    jal SYS_PUTC
#    ori $a0, $zero, 108
#    jal SYS_PUTC
#    ori $a0, $zero, 108
#    jal SYS_PUTC
#    ori $a0, $zero, 111
#    jal SYS_PUTC
#    j TEST
#END:
#    j END*


#TEST:
#    addi $t0, $zero, 3000
#    sw $t0, GLBVAR SCR_BASE

#    addi $a0, $zero, 34
#    addi $a1, $zero, 79
#    jal GOTOXY
#    add $s0, $zero, $zero
#    addi $s1, $zero, 26
#FOR:    addi $t0, $s0, 97
#    add $a0, $zero, $t0
#    jal PUTC
#    addi $s0, $s0, 1
#    blt $s0, $s1, FOR
#END:    j END
