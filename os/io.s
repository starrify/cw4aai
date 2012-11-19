.inc "syscall.inc"


PUTC: #ch
        syscall SC_PUTC
        jr $ra

GETC:
        syscall SC_GETC
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