SYS_GOTOXY: #row, col
        #boundary check
        ori $t0, $zero, SCR_HEIGHT
        bgeu $a0, $t0, SYS_GXYERR
        ori $t0, $zero, SCR_WIDTH
        bgeu $a1, $t0, SYS_GXYERR

        #calculate cursor position
        sll $t1, $a0, 6
        sll $t0, $a0, 4
        add $t0, $t0, $t1
        add $t0, $t0, $a1
        #four bytes per unit
        sll $t0, $t0, 2

        #decide real offset
        SYSINFO_L $t1, SCR_OFFSET
        add $t0, $t0, $t1
        ori $t1, $zero, 4 * SCR_WIDTH * SCR_HEIGHT
        blt $t0, $t1, SYS_GXY1
        addi $t0, $t0, -4 * SCR_WIDTH * SCR_HEIGHT

SYS_GXY1:
        SYSINFO_S $t0, CURSOR
        and $v0, $zero, $zero
        j SYS_GXYEND
        
SYS_GXYERR:
        addi $v0, $zero, -1        
SYS_GXYEND:
        jr $ra

SYS_CURSORNEXT:
        addi $sp, $sp, -4
        sw $ra, 0($sp)

        SYSINFO_L $t1, CURSOR
        #move cursor
        addi $t1, $t1, 4
        ori $t0, $zero, 4 * SCR_WIDTH * SCR_HEIGHT
        blt $t1, $t0, SYS_CSNXT1
        addi $t1, $t1, -4 * SCR_WIDTH * SCR_HEIGHT
SYS_CSNXT1:
        SYSINFO_S $t1, CURSOR

        SYSINFO_L $t2, SCR_OFFSET
        bne $t1, $t2, SYS_CSNXT3

        #scroll
        addi $t2, $t2, 4 * SCR_WIDTH
        blt $t2, $t0, SYS_CSNXT2
        addi $t2, $t2, -4 * SCR_WIDTH * SCR_HEIGHT
SYS_CSNXT2:
        SYSINFO_S $t2, SCR_OFFSET
        #clear the new row
        add $a0, $zero, $t2
        ori $a1, $zero, 32
        ori $a2, $zero, 4 * SCR_WIDTH
        jal MEMSET

SYS_CSNXT3:
        lw $ra, 0($sp)
        addi $sp, $sp, 4
        jr $ra     

SYS_PUTC: #ch
        addi $sp, $sp, -4
        sw $ra, 0($sp)
        
        SYSINFO_L $t0, SCR_BASE
        SYSINFO_L $t1, CURSOR

        #write to disp mem
        add $t0, $t0, $t1
        sw $a0, 0($t0)
        jal SYS_CURSORNEXT

        lw $ra, 0($sp)
        addi $sp, $sp, 4
        jr $ra

SYS_GETC:
        SYSINFO_L $t0, INBUF_START
        SYSINFO_L $t1, INBUF_END
        bne $t0, $t1, SYS_GTCAVL
        #return?
        and $v0, $zero, $zero
        j SYS_GTCEND

        #block?
        #j SYS_GETC

SYS_GTCAVL:
        SYSINFO_L $t1, INBUF_BASE
        add $t1, $t0, $t1
        lbu $v0, 0($t1)
        addi $t0, $t0, 1
        ori $t1, $zero, INBUF_SIZE
        blt $t0, $t1, SYS_GTC1
        addi $t0, $t0, -INBUF_SIZE

SYS_GTC1:
        SYSINFO_S $t0, INBUF_START
SYS_GTCEND:
        jr $ra

.inc "string.s"