
INIT:
    ori $sp, $zero, 0x8000
    ori $a0, $zero, 34
    ori $a1, $zero, 78
    jal SYS_GOTOXY
    sll $zero, $zero, 0
    ori $a0, $zero, 104
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 101
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 108
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 108
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 111
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 34
    ori $a1, $zero, 78
    jal SYS_GOTOXY
    sll $zero, $zero, 0
TEST:
    ori $a0, $zero, 104
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 101
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 108
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 108
    jal SYS_PUTC
    sll $zero, $zero, 0
    ori $a0, $zero, 111
    jal SYS_PUTC
    sll $zero, $zero, 0
    j TEST
    sll $zero, $zero, 0
END:
    j END
    sll $zero, $zero, 0
SYS_PUTC:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    ori $at, $zero, 32768
    lw $t0, 48($at)
    ori $at, $zero, 32768
    lw $t1, 56($at)
    add $t0, $t0, $t1
    sw $a0, 0($t0)
    jal SYS_CURSORNEXT
    sll $zero, $zero, 0
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    sll $zero, $zero, 0
SYS_GOTOXY:
    ori $t0, $zero, 35
    sltu $at, $a0, $t0
    beq $at, $zero, SYS_GXYERR
    sll $zero, $zero, 0
    ori $t0, $zero, 80
    sltu $at, $a1, $t0
    beq $at, $zero, SYS_GXYERR
    sll $zero, $zero, 0
    sll $t1, $a0, 6
    sll $t0, $a0, 4
    add $t0, $t0, $t1
    add $t0, $t0, $a1
    sll $t0, $t0, 2
    ori $at, $zero, 32768
    lw $t1, 52($at)
    add $t0, $t0, $t1
    ori $t1, $zero, 4 * 80 * 35
    slt $at, $t0, $t1
    bne $at, $zero, SYS_GXY1
    sll $zero, $zero, 0
    addi $t0, $t0, -4 * 80 * 35
SYS_GXY1:
    ori $at, $zero, 32768
    sw $t0, 56($at)
    and $v0, $zero, $zero
    j SYS_GXYEND
    sll $zero, $zero, 0
SYS_GXYERR:
    addi $v0, $zero, -1
SYS_GXYEND:
    jr $ra
    sll $zero, $zero, 0
SYS_CURSORNEXT:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    ori $at, $zero, 32768
    lw $t1, 56($at)
    addi $t1, $t1, 4
    ori $t0, $zero, 4 * 80 * 35
    slt $at, $t1, $t0
    bne $at, $zero, SYS_CSNXT1
    sll $zero, $zero, 0
    addi $t1, $t1, -4 * 80 * 35
SYS_CSNXT1:
    ori $at, $zero, 32768
    sw $t1, 56($at)
    ori $at, $zero, 32768
    lw $t2, 52($at)
    bne $t1, $t2, SYS_CSNXT3
    sll $zero, $zero, 0
    addi $t2, $t2, 4 * 80
    slt $at, $t2, $t0
    bne $at, $zero, SYS_CSNXT2
    sll $zero, $zero, 0
    addi $t2, $t2, -4 * 80 * 35
SYS_CSNXT2:
    ori $at, $zero, 32768
    sw $t2, 52($at)
    add $a0, $zero, $t2
    ori $a1, $zero, 32
    ori $a2, $zero, 4 * 80
    jal MEMSET
    sll $zero, $zero, 0
SYS_CSNXT3:
    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra
    sll $zero, $zero, 0
MEMSET:
    sll $a1, $a1, 24
    srl $a1, $a1, 24
    or $t0, $zero, $a1
    sll $t0, $t0, 8
    or $t1, $t0, $a1
    sll $t0, $t1, 16
    or $t0, $t0, $t1
MST1:
    addi $a2, $a2, -4
    slt $at, $a2, $zero
    bne $at, $zero, MST2
    sll $zero, $zero, 0
    sw $t0, 0($a0)
    addi $a0, $a0, 4
    j MST1
    sll $zero, $zero, 0
MST2:
    addi $a2, $a2, 4
MST3:
    beq $a2, $zero, MSTEND
    sll $zero, $zero, 0
    sb $a1, 0($a0)
    addi $a0, $a0, 1
    addi $a2, $a2, -1
    j MST3
    sll $zero, $zero, 0
MSTEND:
    jr $ra
    sll $zero, $zero, 0
