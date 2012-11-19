
INIT:
        ori $sp, $zero, 0x8000
        ori $a0, $zero, 34
        ori $a1, $zero, 78
        jal SYS_GOTOXY
        sll $zero, $zero, 0
        ori $a0, $zero, 104
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 101
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 108
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 108
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 111
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 34
        ori $a1, $zero, 78
        jal SYS_GOTOXY
        sll $zero, $zero, 0
TEST:
        ori $a0, $zero, 104
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 101
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 108
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 108
        jal SYS_PUTC
        sll $zero, $zero, 0
        ori $a0, $zero, 111
        jal SYS_PUTC
        sll $zero, $zero, 0
        j TEST
        sll $zero, $zero, 0
END:
        j END
        sll $zero, $zero, 0
SYS_GOTOXY:
        ori $t0, $zero, 35
	sltu $at, $a0, $t0
        beq $at, $zero, SYS_GXYERR
        sll $zero, $zero, 0
        ori $t0, $zero, 80
	sltu $at, $a1, $t0
        beq $at, $zero, SYS_GXYERR
        sll $zero, $zero, 0
        sll $t1, $a0, 6
        sll $t0, $a0, 4
        add $t0, $t0, $t1
        add $t0, $t0, $a1
        sll $t0, $t0, 2
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t1, 52($at)
        add $t0, $t0, $t1
        ori $t1, $zero, 4 * 80 * 35
	slt $at, $t0, $t1
        bne $at, $zero, SYS_GXY1
        sll $zero, $zero, 0
        addi $t0, $t0, -4 * 80 * 35
SYS_GXY1:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        sw $t0, 56($at)
        and $v0, $zero, $zero
        j SYS_GXYEND
        sll $zero, $zero, 0
SYS_GXYERR:
        addi $v0, $zero, -1
SYS_GXYEND:
        jr $ra
        sll $zero, $zero, 0
SYS_CURSORNEXT:
        addi $sp, $sp, -4
        sw $ra, 0($sp)
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t1, 56($at)
        addi $t1, $t1, 4
        ori $t0, $zero, 4 * 80 * 35
	slt $at, $t1, $t0
        bne $at, $zero, SYS_CSNXT1
        sll $zero, $zero, 0
        addi $t1, $t1, -4 * 80 * 35
SYS_CSNXT1:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        sw $t1, 56($at)
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t2, 52($at)
        bne $t1, $t2, SYS_CSNXT3
        sll $zero, $zero, 0
        addi $t2, $t2, 4 * 80
	slt $at, $t2, $t0
        bne $at, $zero, SYS_CSNXT2
        sll $zero, $zero, 0
        addi $t2, $t2, -4 * 80 * 35
SYS_CSNXT2:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        sw $t2, 52($at)
        add $a0, $zero, $t2
        ori $a1, $zero, 32
        ori $a2, $zero, 4 * 80
        jal MEMSET
        sll $zero, $zero, 0
SYS_CSNXT3:
        lw $ra, 0($sp)
        addi $sp, $sp, 4
        jr $ra
        sll $zero, $zero, 0
SYS_PUTC:
        addi $sp, $sp, -4
        sw $ra, 0($sp)
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t0, 48($at)
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t1, 56($at)
        add $t0, $t0, $t1
        sw $a0, 0($t0)
        jal SYS_CURSORNEXT
        sll $zero, $zero, 0
        lw $ra, 0($sp)
        addi $sp, $sp, 4
        jr $ra
        sll $zero, $zero, 0
SYS_GETC:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t0, 68($at)
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t1, 72($at)
        bne $t0, $t1, SYS_GTCAVL
        sll $zero, $zero, 0
        and $v0, $zero, $zero
        j SYS_GTCEND
        sll $zero, $zero, 0
SYS_GTCAVL:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        lw $t1, 64($at)
        add $t1, $t0, $t1
        lbu $v0, 0($t1)
        addi $t0, $t0, 1
        ori $t1, $zero, 4096
	slt $at, $t0, $t1
        bne $at, $zero, SYS_GTC1
        sll $zero, $zero, 0
        addi $t0, $t0, -4096
SYS_GTC1:
        lui $at, (32768 >> 16) & 0xffff
        ori $at, $zero, 32768 & 0xffff
        sw $t0, 68($at)
SYS_GTCEND:
        jr $ra
        sll $zero, $zero, 0
MEMCPY:
        add $t1, $a0, $a2
        sltu $t0, $a1, $t1
        add $t1, $a1, $a2
        sltu $t2, $a0, $t1
        and $t0, $t0, $t2
        beq $t0, $zero, MCPCTN
        sll $zero, $zero, 0
        addi $v0, $zero, -1
        j MCPEND
        sll $zero, $zero, 0
MCPCTN:
        and $t0, $zero, $zero
MCPFOR:
        add $t1, $a1, $t0
        lb $t2, 0($t1)
        add $t1, $a0, $t0
        sb $t2, 0($t1)
        addi $t0, $t0, 1
	slt $at, $t0, $a2
        bne $at, $zero, MCPFOR
        sll $zero, $zero, 0
        add $v0, $zero, $zero
MCPEND:
        jr $ra
        sll $zero, $zero, 0
STRCPY:
SCPLP:
        lb $t0, 0($a1)
        beq $t0, $zero, SCPEND
        sll $zero, $zero, 0
        sb $t0, 0($a0)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        j SCPLP
        sll $zero, $zero, 0
SCPEND:
        jr $ra
        sll $zero, $zero, 0
MEMCMP:
        and $t0, $zero, $zero
        and $t1, $zero, $zero
MCMPLP:
        beq $a2, $zero, MCMPEND
        sll $zero, $zero, 0
        lb $t0, 0($a0)
        lb $t1, 0($a1)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        addi $a2, $a2, -1
        beq $t0, $t1, MCMPLP
        sll $zero, $zero, 0
MCMPEND:
        sub $v0, $t0, $t1
        jr $ra
        sll $zero, $zero, 0
STRCMP:
SCMPLP:
        lb $t0, 0($a0)
        lb $t1, 0($a1)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        beq $t0, $t1, SCMP1
        sll $zero, $zero, 0
        j SCMPEND
        sll $zero, $zero, 0
SCMP1:
        bne $t0, $zero, SCMPLP
        sll $zero, $zero, 0
SCMPEND:
        sub $v0, $t0, $t1
        jr $ra
        sll $zero, $zero, 0
MEMSET:
MEMSETLP:
        beq $a2, $zero, MSTEND
        sll $zero, $zero, 0
        sb $a1, 0($a0)
        addi $a0, $a0, 1
        addi $a2, $a2, -1
        j MEMSETLP
        sll $zero, $zero, 0
MSTEND:
        jr $ra
        sll $zero, $zero, 0
