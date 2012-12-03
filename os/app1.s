.inc "jmp_fix.inc"
.inc "syscall.inc"
.inc "io.lib"

.def nop {
    sll $zero, $zero, 0
}

MAIN:
    ori $k0, $zero, SC_FORK
    syscall
    beq $v0, $zero, CHILD
    nop
FATHER:
    ori $a0, $zero, 97
    jal PUTC
    nop
    wait
    j FATHER
    nop
CHILD:
    ori $a0, $zero, 98
    jal PUTC
    nop
    #wait
    j CHILD
    nop
END:
    ori $k0, $zero, SC_EXIT
    syscall
    j END
    nop

    
