.inc "jmp_fix.inc"
.inc "io.lib"

.def b LABEL {
    beq $zero, $zero, LABEL
}
.def nop {
    sll $zero, $zero, 0
}

    ori $k0, $zero, SC_FORK
    syscall

    beq $zero, $v0, PPROC
    nop
    b CPROC
    nop

PPROC:
    ori $t0, $zero, 0x61
    or $a0, $zero, $t0
    jal PUTC
    b OUTPUT
    nop

CPROC:
    ori $t0, $zero, 0x62
    or $a0, $zero, $t0
    jal PUTC
    b OUTPUT
    nop

OUTPUT:
#    or $a0, $zero, $t0
#    jal PUTC
    nop
    ori $a0, $zero, 0xa
#    jal PUTC
    nop

#    b OUTPUT
    nop
    b QUIT
    nop

QUIT:
    ori $k0, $zero, 0 #SC_EXIT
    #syscall
    b QUIT

