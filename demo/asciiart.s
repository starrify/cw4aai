.inc "../inc/jmp_fix.inc"
.inc "../inc/syscall.inc"
.inc "../lib/io.lib"
.inc "../lib/string.lib"

.def b param0 {
    beq $zero, $zero, param0
}

.def nop {
    sll $zero, $zero, 0
}

.def MSC_DATA_OFF {0}
.def MSC_DATA_SIZE {0x20000} #128k in bytes

.static {
MSC_FILENAME:
    .str "earth.dat"
ERRMSG:
    .str "cannot open file earth.dat\n"
}

    lla $s0, STATIC_END
    addi $s0, $s0, MSC_DATA_OFF
    lla $a0, MSC_FILENAME
    or $a1, $zero, $s0
    li $a2, MSC_DATA_SIZE
    ori $k0, $zero, SC_READ
    syscall
    beq $v0, $zero, MAIN
    lla $a0, ERRMSG
    jal PUTS
    j EXIT

MAIN:
    lw $t0, 0($s0)  # max char width, unused
    lw $s1, 4($s0)  # line count
    lw $s2, 8($s0)  # frame count
    lw $s3, 12($s0) # frame duration

    addi $a0, $s0, 16
    #lla $a0, NL
    jal PUTS    # flush screen once
    nop
    li $k0, SC_GETXY
    syscall
    sub $s6, $v0, $s1   # starting x. and starting y is $v1
    add $s7, $zero, $v1

DRAW_FRAME_RESTART:
    li $s5, 0   # loop counter
    addi $s4, $s0, 16   # string base
DRAW_FRAME_LOOP:
    add $a0, $zero, $s6
    add $a1, $zero, $s7
    li $k0, SC_GOTOXY    # restore the original cursor position
    syscall 
    add $a0, $zero, $s4
    jal PUTS
    nop
    add $a0, $zero, $s3
    li $k0, SC_SLEEP    # well just let it sleep for a while..
    syscall
    addi $s5, $s5, 1
    bge $s5, $s2, DRAW_FRAME_RESTART
    nop
    add $a0, $zero, $s4
    jal STRLEN
    nop
    addi $v0, $v0, 1
    xori $0, $0, 0
    sw $v0, 0($t0)
    li $t0, 4
    mult $t0, $v0
    mflo $t0
    sw $t0, 0($t0)
    add $s4, $s4, $t0
    b DRAW_FRAME_LOOP
    nop

EXIT:
    ori $k0, $zero, SC_EXIT
    syscall

