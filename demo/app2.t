
BEG:
    ori $a0, $zero, 98
    jal PUTC
    sll $zero, $zero, 0
    wait
    j BEG
    sll $zero, $zero, 0
END:
    j END
    sll $zero, $zero, 0
PUTC:
    ori $k0, $zero, 3
    syscall
    jr $ra
    sll $zero, $zero, 0
STATIC_BEG:
STATIC_END:
