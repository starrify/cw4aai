
SYS:
        jal INIT
        jal SHELL

INIT:
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
        addi $a0, $zero, 10
        jal PUTC
        j SHLLP
        
        addi $sp, $sp, 80
        jr $ra

.inc "io.s"
