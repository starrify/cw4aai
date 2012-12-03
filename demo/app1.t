
MAIN:
    lla $a0, STRHELLO
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
STATIC_BEG:
STRHELLO:
    .str "hello world"
STATIC_END:
