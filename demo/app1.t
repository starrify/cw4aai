
MAIN:
    lla $a0, MEM1
    jal GETS
    sll $zero, $zero, 0
    lla $a0, MEM1
    jal PUTS
    sll $zero, $zero, 0
END:
    j END
    sll $zero, $zero, 0
    PUTS:
    addi $sp, $sp, -8
    sw $ra, 4($sp)
    sw $s0, 0($sp)
    add $s0, $zero, $a0
PTSLP:
    lw $a0, 0($s0)
    beq $a0, $zero, PTSEND
    sll $zero, $zero, 0
    jal PUTC
    sll $zero, $zero, 0
    addi $s0, $s0, 4
    j PTSLP
    sll $zero, $zero, 0
PTSEND:
    lw $s0, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra
    sll $zero, $zero, 0
PUTC:
    ori $k0, $zero, 3
    syscall
    jr $ra
    sll $zero, $zero, 0
GETS:
    addi $sp, $sp, -12
    sw $ra, 0($sp)
    sw $s0, 4($sp)
    sw $s1, 8($sp)
    ori $s1, $zero, 10
    add $s0, $zero, $a0
GTSLP:
    jal GETC
    sll $zero, $zero, 0
    sw $v0, 0($s0)
    addi $s0, $s0, 4
    beq $v0, $s1, GTSEND
    sll $zero, $zero, 0
    j GTSLP
    sll $zero, $zero, 0
GTSEND:
    sw $zero, 0($s0)
    add $v0, $zero, $s0
    lw $s1, 8($sp)
    lw $s0, 4($sp)
    lw $ra, 0($sp)
    addi $sp, $sp, 12
    jr $ra
    sll $zero, $zero, 0
GETC:
GTCBLK:
    ori $k0, $zero, 4
    syscall
    slt $at, $v0, $zero
    beq $at, $zero, GTCEND
    sll $zero, $zero, 0
    wait
    j GTCBLK
    sll $zero, $zero, 0
GTCEND:
    jr $ra
    sll $zero, $zero, 0
STATIC_BEG:
STRHELLO:
    .str "hello world"
MEM1:
    .dup 80 .0
STATIC_END:
