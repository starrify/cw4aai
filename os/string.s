MEMCPY: #dst, src, size
        #check if overlap
        add $t1, $a0, $a2
        sltu $t0, $a1, $t1
        add $t1, $a1, $a2
        sltu $t2, $a0, $t1
        and $t0, $t0, $t2
        #jump to MCPCTN if (a0 + a2 > a1 and a1 + a2 > a0) == 0
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
        blt $t0, $a2, MCPFOR

        add $v0, $zero, $zero
        jr $ra

STRCPY: #dst, src
SCPLP:
        lb $t0, 0($a1)
        beq $t0, $zero, SCPEND
        sb $t0, 0($a0)
        addi $a0, $a0, 1
        addi $a1, $a1, 1
        j SCPLP

SCPEND:
        jr $ra

MEMCMP: #str1, str2, size
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

STRCMP: #str1, str2
        
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

MEMSET: #pt, value, size
        beq $a2, $zero, MSTEND
        sb $a1, 0($a0)
        addi $a0, $a0, 1
        addi $a2, $a2, -1
        j MEMSET
MSTEND:
        jr $ra