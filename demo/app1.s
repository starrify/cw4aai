.inc "../inc/jmp_fix.inc"
.inc "../inc/syscall.inc"
.inc "../lib/io.lib"

.static {
STRHELLO:
    .str "hello world\n"
MEM1:
    .dup 80 .0
}

MAIN:
    #jal GETC
    #or $a0, $zero, $v0
    #addi $zero, $zero, 0
    #lla $a0, STRHELLO
    #lla $a0, MEM1
    #jal GETS
    lla $a0, STRHELLO
    jal PUTS
    ori $a0, $zero, 102
    ori $k0, $zero, SC_SLEEP
    syscall

    li $t0, 0x01808000  # sbase in kernel address space
    li $t1, 2   # 1 for shutting down and 2 for resetting
    sw $t1, 0($t0)  # trigger hardware shutdown/reset..
    # this line shall not be executed
    j MAIN
    #ori $k0, $zero, SC_EXIT
    #syscall
END:
    wait
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

    
