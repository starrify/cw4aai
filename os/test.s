.inc "pseudo_inst.inc"
.inc "var.inc"
.inc "jmp_fix.inc"
.inc "syscall.inc"
.inc "io.lib"

.offset 0x800000

.static {
CUR_USR_PROC:
    .0
PROC_SEGBASE:    #starting from system
    .dup 32 .0
PROC_CURSP:      #starting from system
    .dup 32 .0
}

INIT:
    ori $sp, $zero, 0x8000      #j
    lla $t0, WTF
    srl $t0, $t0, 2
    lui $t1, 0x0800
    or $t0, $t0, $t1
    swia $t0, 0x80, 0
    ori $t0, $zero, 1
    #ori $t1, $zero, 3
    mtc0 $t0, $3
    #interrupt table
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
