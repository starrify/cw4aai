.inc "pseudo_inst.inc"
.inc "var.inc"
.inc "jmp_fix.inc"
.inc "syscall.lib"

.dup 0x1000 >> 2 .0

LOOP:
    jal SYS_GETC
    blt $v0, $zero, LOOP
    or $a0, $zero, $v0
    jal SYS_PUTC
    j LOOP

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
