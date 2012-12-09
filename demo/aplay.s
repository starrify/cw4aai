.inc "../inc/jmp_fix.inc"
.inc "../inc/syscall.inc"
.inc "../lib/io.lib"

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
    .str "smbtheme"
ERRMSG:
    .str "cannot open file smbtheme\n"
}

INIT:
    lla $s0, STATIC_END
    addi $s0, $s0, MSC_DATA_OFF
    #load music data from file
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
    li $t0, 0x01808000  # sbase in kernel address space
LOOP:
    lw $t2, 0($s0)  # freq in MHz
    beq $t2, $zero, EXIT    # section ending 
    sw $t2, 0xC4($t0)   
    lw $t2, 4($s0)  # dura in ms
    sw $t2, 0xC8($t0)   
    li $t3, 1
    sw $t3, 0xC0($t0)   # trigger PC speaker play
    lw $t3, 8($s0)  # wait in ms
    add $a0, $t2, $t3
    li $k0, SC_SLEEP
    syscall
    addi $s0, $s0, 16
    b LOOP
    nop
EXIT:
    ori $k0, $zero, SC_EXIT
    syscall

