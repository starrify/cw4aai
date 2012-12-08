.inc "../inc/jmp_fix.inc"
.inc "../lib/io.lib"
.inc "shell.lib"

SHELL:
    #local variable
    .def LOCAL_SHELL_CMD_OFF {0}
    addi $sp, $sp, -4 * 80
SHELLLP:
    lla $a0, SH_PROMPT
    jal PUTS
    addi $a0, $sp, LOCAL_SHELL_CMD_OFF
    jal GETS
    or $a0, $zero, $v0
    jal SH_CMD
    j SHELL

SHELLEND:
    addi $sp, $sp, 4 * 80
    