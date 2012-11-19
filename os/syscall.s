SYS_GOTOXY: #row, col
        #boundary check
        addi $t0, $zero, SCR_HEIGHT
        bgeu $a0, $t0, SYS_GXYERR
        addi $t0, $zero, SCR_WIDTH
        bgeu $a1, $t0, SYS_GXYERR

        #calculate cursor position
        sll $t1, $a0, 6
        sll $t0, $a0, 4
        add $t0, $t0, $t1
        add $t0, $t0, $a1
        #two bytes per unit
        sll $t0, $t0, 1

        #decide real offset
        lw $t1, GLBVAR SCR_OFFSET
        add $t0, $t0, $t1
        addi $t1, $zero, 2 * SCR_WIDTH * SCR_HEIGHT
        blt $t0, $t1, SYS_GXY1
        addi $t0, $t0, -2 * SCR_WIDTH * SCR_HEIGHT

SYS_GXY1:
        sw $t0, GLBVAR CURSOR
        and $v0, $zero, $zero
        jr $ra
SYS_GXYERR:
        addi $v0, $zero, -1
        jr $ra

SYS_PUTC: #ch
        lw $t0, GLBVAR SCR_BASE
        lw $t1, GLBVAR CURSOR

        #write to disp mem
        add $t0, $t0, $t1
        sb $a0, 0($t0)
        srl $a0, $a0, 8
        sb $a0, 1($t0)

        #move cursor
        addi $t1, $t1, 2
        addi $t0, $zero, 2 * SCR_WIDTH * SCR_HEIGHT
        blt $t1, $t0, SYS_PTC1
        addi $t1, $t1, -2 * SCR_WIDTH * SCR_HEIGHT
SYS_PTC1:
        sw $t1, GLBVAR CURSOR

        lw $t2, GLBVAR SCR_OFFSET
        bne $t1, $t2, SYS_PTC3

        #scroll
        addi $t2, $t2, 2 * SCR_WIDTH
        blt $t2, $t0, SYS_PTC2
        addi $t2, $t2, -2 * SCR_WIDTH * SCR_HEIGHT
        #clear the new row
        add $a0, $zero, $t2
        and $a1, $zero, $zero
        addi $a2, $zero, 2 * SCR_WIDTH
        jal MEMSET
SYS_PTC2:
        sw $t2, GLBVAR SCR_OFFSET

SYS_PTC3:
        jr $ra

SYS_GETC:
        lw $t0, GLBVAR INBUF_START
        lw $t1, GLBVAR INBUF_END
        bne $t0, $t1, SYS_GTCAVL
        #return?
        #addi $v0, $zero, -1
        #jr $ra

        #block?
        j SYS_GETC

SYS_GTCAVL:
        lw $t1, GLBVAR INBUF_BASE
        add $t1, $t0, $t1
        lbu $v0, 0($t1)
        addi $t0, $t0, 1
        addi $t1, $zero, INBUF_SIZE
        blt $t0, $t1, SYS_GTC1
        addi $t0, $t0, -INBUF_SIZE

SYS_GTC1:
        sw $t0, GLBVAR INBUF_START
        jr $ra

.inc "string.s"