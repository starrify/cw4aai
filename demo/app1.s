.inc "jmp_fix.inc"
.inc "io.lib"

MAIN:
    ori $k0, $zero, SC_FORK
    syscall
    beq $v0, $zero, CHILD
FATHER:
    ori $a0, $zero, 97
    jal PUTC
#    wait
    j FATHER
CHILD:
    ori $a0, $zero, 98
    jal PUTC
    #wait
    j CHILD
END:
    j END

    