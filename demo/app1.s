.inc "../inc/jmp_fix.inc"
.inc "../inc/syscall.inc"
.inc "../lib/io.lib"

.static {
STRHELLO:
    .str "hello world"
}

MAIN:
    #jal GETC
    #or $a0, $zero, $v0
    #addi $zero, $zero, 0
    lla $a0, STRHELLO
    jal PUTS
END:
    j END
#    ori $k0, $zero, SC_FORK
#    syscall
#    beq $v0, $zero, CHILD
#FATHER:
#    ori $a0, $zero, 97
#    jal PUTC
#    wait
#    j FATHER
#CHILD:
#    ori $a0, $zero, 98
#    jal PUTC
    #wait
#    j CHILD
#.def nop {
#    sll $zero, $zero, 0
#}

#MAIN:
#    ori $k0, $zero, SC_FORK
#    syscall
#    beq $v0, $zero, CHILD
#    nop
#FATHER:
#    ori $a0, $zero, 97
#    jal PUTC
#    nop
#    wait
#    j FATHER
#    nop
#CHILD:
#    ori $a0, $zero, 98
#    jal PUTC
#    nop
    #wait
#    j CHILD
#    nop
#END:
#    ori $k0, $zero, SC_EXIT
#    syscall
#    j END
#    nop

    
