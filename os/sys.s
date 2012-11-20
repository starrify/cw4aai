.inc "io."

SYS:
    jal INIT
    jal SHELL
    #syscall exit

INIT:
    addi $sp, $sp, -4
    sw $ra, 0($sp)
    #register input function
    #initialize library
    jal LIB_INIT

    lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra

SHELL:
    addi $sp, $sp, -80
    #cmd[80]
    .def shell_cmd {0}

SHLLP:
    add $a0, $sp, shell_cmd
    jal GETS
    add $a0, $sp, shell_cmd
    jal PUTS
    ori $a0, $zero, 10
    jal PUTC
    j SHLLP
    
    addi $sp, $sp, 80
    jr $ra
