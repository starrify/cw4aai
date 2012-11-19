.inc "syscall.inc"
.inc "var.inc"

PUTC: #ch
        syscall SC_PUTC
        jr $ra

GETC:
GTCBLK:
        OSVAR_L $t0, LIB_INBUF_START
        OSVAR_L $t1, LIB_INBUF_END
        beq $t0, $t1, GTCBLK

        #read from lib buf
        OSVAR_L $t1, LIB_INBUF_BASE
        add $t1, $t0, $t1
        lw $v0, 0($t1)
        
        #move lib buf start
        addi $t0, $t0, 1
        addi $t1, $zero, LIB_INBUF_SIZE
        blt $t0, $t1, GTC1
        addi $t0, $t0, -LIB_INBUF_SIZE
GTC1:
        OSVAR_S $t0, LIB_INBUF_START

        jr $ra

PUTS: #pt
        addi $sp, $sp, -4
        sw $s0, 0($sp)
        
	add $s0, $zero, $a0
PTSLP:
        lw $a0, 0($s0)
	beq $a0, $zero, PTSEND
	jal PUTC
	addi $s0, $s0, 1
	j PTSLP

PTSEND:
        lw $s0, 0($sp)
        addi $sp, $sp, 4
        jr $ra

GETS: #pt
        addi $sp, $sp, -4
        sw $s0, 0($sp)
        
        addi $t0, $zero, 10
        add $s0, $zero, $a0
GTSLP:
        jal GETC
        beq $v0, $t0, GTSEND
        sw $v0, 0($s0)
        addi $s0, $s0, 1
        j GTSLP

GTSEND:
        #add trailing zero
        sw $zero, 0($s0)

        add $v0, $zero, $s0
        lw $s0, 0($sp)
        addi $sp, $sp, 4
        jr $ra