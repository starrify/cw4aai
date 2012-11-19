





































 


INIT:
        addiu $t0, $zero, 63536
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        sw $t0, 48($at)


TEST:
        addi $a0, $zero, 104
        jal SYS_PUTC
        addi $a0, $zero, 101
        jal SYS_PUTC
        addi $a0, $zero, 108
        jal SYS_PUTC
        addi $a0, $zero, 108
        jal SYS_PUTC
        addi $a0, $zero, 111
        jal SYS_PUTC
        j TEST
END:
        j END

SYS_GOTOXY: 
        
        addi $t0, $zero, 35
        
	sltu	$at, $a0, $t0
	beq	$at, $zero, SYS_GXYERR

        addi $t0, $zero, 80
        
	sltu	$at, $a1, $t0
	beq	$at, $zero, SYS_GXYERR


        
        sll $t1, $a0, 6
        sll $t0, $a0, 4
        add $t0, $t0, $t1
        add $t0, $t0, $a1
        
        sll $t0, $t0, 2

        
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t1, 52($at)

        add $t0, $t0, $t1
        addi $t1, $zero, 4 * 80 * 35
        
	slt	$at, $t0, $t1
	bne	$at, $zero, SYS_GXY1

        addi $t0, $t0, -4 * 80 * 35

SYS_GXY1:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        sw $t0, 56($at)

        and $v0, $zero, $zero
        jr $ra
SYS_GXYERR:
        addi $v0, $zero, -1
        jr $ra

SYS_PUTC: 
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t0, 48($at)

        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t1, 56($at)


        
        add $t0, $t0, $t1
        sw $a0, 0($t0)
        
        
        

        
        addi $t1, $t1, 4
        addi $t0, $zero, 4 * 80 * 35
        
	slt	$at, $t1, $t0
	bne	$at, $zero, SYS_PTC1

        addi $t1, $t1, -4 * 80 * 35
SYS_PTC1:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        sw $t1, 56($at)


        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t2, 52($at)

        bne $t1, $t2, SYS_PTC3

        
        addi $t2, $t2, 4 * 80
        
	slt	$at, $t2, $t0
	bne	$at, $zero, SYS_PTC2

        addi $t2, $t2, -4 * 80 * 35
        
        add $a0, $zero, $t2
        andi $a1, $zero, 32
        addi $a2, $zero, 4 * 80
        jal MEMSET
SYS_PTC2:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        sw $t2, 52($at)


SYS_PTC3:
        jr $ra

SYS_GETC:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t0, 68($at)

        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t1, 72($at)

        bne $t0, $t1, SYS_GTCAVL
        
        addi $v0, $zero, -1
        jr $ra

        
        

SYS_GTCAVL:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        lw $t1, 64($at)

        add $t1, $t0, $t1
        lbu $v0, 0($t1)
        addi $t0, $t0, 1
        addi $t1, $zero, 4096
        
	slt	$at, $t0, $t1
	bne	$at, $zero, SYS_GTC1

        addi $t0, $t0, -4096

SYS_GTC1:
        
        
        lui     $at, (32768 >> 16) & 0xff
        addi    $at, $zero, 32768 & 0xff

        sw $t0, 68($at)

        jr $ra

MEMCPY: 
        
        add $t1, $a0, $a2
        sltu $t0, $a1, $t1
        add $t1, $a1, $a2
        sltu $t2, $a0, $t1
        and $t0, $t0, $t2
        
        beq $t0, $zero, MCPCTN
        addi $v0, $zero, -1
        jr $ra

MCPCTN:
        and $t0, $zero, $zero
MCPFOR:
        add $t1, $a1, $t0
        lb $t2, 0($t1)
        add $t1, $a0, $t0
        sb $t2, 0($t1)
        addi $t0, $t0, 1
        
	slt	$at, $t0, $a2
	bne	$at, $zero, MCPFOR


        add $v0, $zero, $zero
        jr $ra

STRCPY: 
SCPLP:
        lb $t0, 0($a1)
        beq $t0, $zero, SCPEND
        sb $t0, 0($a0)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        j SCPLP

SCPEND:
        jr $ra

MEMCMP: 
        and $t0, $zero, $zero
        and $t1, $zero, $zero
MCMPLP:
        beq $a2, $zero, MCMPEND
        lb $t0, 0($a0)
        lb $t1, 0($a1)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        addi $a2, $a2, -1
        beq $t0, $t1, MCMPLP
        j MCMPEND

MCMPEND:
        sub $v0, $t0, $t1
        jr $ra

STRCMP: 
        
SCMPLP:
        lb $t0, 0($a0)
        lb $t1, 0($a1)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        beq $t0, $t1, SCMP1
        j SCMPEND
SCMP1:
        bne $t0, $zero, SCMPLP
        j SCMPEND
SCMPEND:
        sub $v0, $t0, $t1
        jr $ra

MEMSET: 
        beq $a2, $zero, MSTEND
        sb $a1, 0($a0)
        addi $a0, $a0, 1
        addi $a2, $a2, -1
        j MEMSET
MSTEND:
        jr $ra
















