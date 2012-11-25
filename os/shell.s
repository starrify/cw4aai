.inc "io.lib"
.inc "interrupt.lib"
.inc "var.inc"

.def SHELL_INBUF_START {0}
.def SHELL_INBUF_END {4}
.def SHELL_INBUF_DATA {8}
.def SHELL_INBUF_SIZE {4096}
.def SHELL_INBUF_UNIT_SIZE {4}
.static {
SHELL_INBUF:
    .0
    .0
    .dup 1024 .0
}


INIT:
    #register keyboard int func for shell
    lla $t0, SHELL_INT_KEYBOARD
    srl $t0, $t0, 2
    lui $t1, 0x0800
    or $t0, $t0, $t1
    swlia $t0, 0x1800080, 0

SHELL:
    addi $sp, $sp, -80
    #cmd[80]
    .def shell_cmd {0}

SHLL1:
    addi $a0, $sp, shell_cmd
    jal GETS
    addi $a0, $sp, shell_cmd
    jal PUTS
    ori $a0, $zero, 10
    jal PUTC
    j SHLL1
    
    addi $sp, $sp, 80
SHLL_END:
    j SHLL_END

    
SHELL_INT_KEYBOARD:
    shade_gpr
    lla $s0, SYS_SP_BUF
    lw $sp, 0($s0)

SHLL_INT_KBD1:
    jal SYS_GETC
    blt $v0, $zero, SHLL_INT_KBDEND
    ori $t0, $zero, 10
    beq $v0, $t0, SHLL_INT_KBD2
    or $a0, $zero, $v0
    jal SHLL_INBUF_PUSHC
    j SHLL_INT_KBD1

#write to io buffer
SHLL_INT_KBD2:
    jal SHELL_INBUF_GETC
    blt $v0, $zero, SHLL_INT_KBD3
    or $a0, $zero, $v0
    jal PUSHC
SHLL_INT_KBD3:
    ori $a0, $zero, 10
    jal PUSHC
    
SHLL_INT_KBDEND:
    sw $sp, 0($s0)
    restore_gpr
    eret

SHELL_INBUF_GETC:
    SETFRMLB SHELL_INBUF
    lw $t0, SHELL_INBUF_START($fp)
    lw $t1, SHELL_INBUF_END($fp)
    bne $t0, $t1, SHLL_INBUF_GTC1
    addi $v0, $zero, -1
    j SHLL_INBUF_GTCEND

SHELL_INBUF_GTC1:
    addi $t1, $fp, SHELL_INBUF_DATA
    add $t1, $t0, $t1
    lw $v0, 0($t1)
    andi $v0, $v0, 0xff
    addi $t0, $t0, SHELL_INBUF_UNIT_SIZE
    ori $t1, $zero, SHELL_INBUF_SIZE
    blt $t0, $t1, SHLL_INBUF_GTC1
    and $t0, $zero, $zero
SHLL_INBUF_GTC1:
    sw $t0, SHELL_INBUF_START($fp)
SHLL_INBUF_GTCEND:
    jr $ra
    
SHLL_INBUF_PUSHC:
    SETFRMLB SHELL_INBUF
    lw $t0, SHELL_INBUF_END($fp)
    addi $t1, $fp, SHELL_INBUF_DATA
    add $t1, $t1, $t0
    sw $a0, 0($t1)
    addi $t0, $t0, 4
    ori $t1, $zero, SHELL_INBUF_SIZE
    bne $t0, $t1, SHLL_INBUF_PSC1
    and $t0, $zero, $zero
SHLL_INBUF_PSC1:
    lw $t1, SHELL_INBUF_START($fp)
    beq $t0, $t1, SHLL_INBUF_PSCERR
    sw $t0, SHELL_INBUF_END($fp)
    and $v0, $zero, $zero
    j SHLL_INBUF_PSCEND
SHLL_INBUF_PSCERR:
    addi $v0, $zero, -1
SHLL_INBUF_PSCEND:
    jr $ra
    