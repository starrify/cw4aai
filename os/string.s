.inc "pseudo_inst.inc"

MEMCPY: #dst, src, size
    #check if overlap
    add $t1, $a0, $a2
    sltu $t0, $a1, $t1
    add $t1, $a1, $a2
    sltu $t2, $a0, $t1
    and $t0, $t0, $t2
    #jump to MCPCTN if (a0 + a2 > a1 and a1 + a2 > a0) == 0
    beq $t0, $zero, MCP1
    j MCPERR
MCP1:
    #store words
    addi $a2, $a2, -4
    blt $a2, $zero, MCP2
    lw $t0, 0($a1)
    sw $t0, 0($a0)
    addi $a0, $a0, 4
    addi $a1, $a1, 4
    j MCP1
MCP2:
    addi $a2, $a2, 4
MCP3:
    addi $a2, $a2, -1
    blt $a2, $zero, MCPEND
    lb $t0, 0($a1)
    sw $t0, 0($a0)
    addi $a0, $a0, 1
    addi $a1, $a1, 1
    j MCP3
MCPEND:
    jr $ra
MCPERR:
    addi $v0, $zero, -1
    j MCPEND
    

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
    #eliminate same substrings using lw, sw
MCMP1:
    addi $a2, $a2, -4
    blt $a2, $zero, MCMP2
    lw $t0, 0($a0)
    lw $t1, 0($a1)
    bne $t0, $t1, MCMP2
    addi $a0, $a0, 4
    addi $a1, $a1, 4
    j MCMP1
MCMP2:
    addi $a2, $a2, 4
MCMP3:
    beq $a2, $zero, MCMPEND
    lb $t0, 0($a0)
    lb $t1, 0($a1)
    addi $a0, $a0, 1
    addi $a1, $a1, 1
    addi $a2, $a2, -1
    beq $t0, $t1, MCMP3

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
SCMPEND:
    sub $v0, $t0, $t1
    jr $ra

MEMSET: #pt, value, size
    #eliminate non-zeros in msbs
    sll $a1, $a1, 24
    srl $a1, $a1, 24
    #calculate 4bytes a1a1a1a1
    or $t0, $zero, $a1
    sll $t0, $t0, 8
    or $t1, $t0, $a1
    sll $t0, $t1, 16
    or $t0, $t0, $t1
MST1:
    addi $a2, $a2, -4
    blt $a2, $zero, MST2
    sw $t0, 0($a0)
    addi $a0, $a0, 4
    j MST1
MST2:
    addi $a2, $a2, 4
MST3:
    beq $a2, $zero, MSTEND
    sb $a1, 0($a0)
    addi $a0, $a0, 1
    addi $a2, $a2, -1
    j MST3
MSTEND:
    jr $ra