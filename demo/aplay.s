.inc "../inc/jmp_fix.inc"
.inc "../inc/syscall.inc"

.def b param0 {
    beq $zero, $zero, param0
}

.def nop {
    sll $zero, $zero, 0
}

MAIN:
    li $t0, 0x01808000  # sbase in kernel address space
    lla $t1, SMB_THEME
LOOP:
    lw $t2, 0($t1)  # freq in MHz
    beq $t2, $zero, EXIT    # section ending 
    sw $t2, 0xC4($t0)   
    lw $t2, 4($t1)  # dura in ms
    sw $t2, 0xC8($t0)   
    li $t3, 1
    sw $t3, 0xC0($t0)   # trigger PC speaker play
    lw $t3, 8($t1)  # wait in ms
    add $a0, $t2, $t3
    li $k0, SC_SLEEP
    syscall
    addi $t1, $t1, 16
    b LOOP
    nop
EXIT:
    ori $k0, $zero, SC_EXIT
    syscall

SMB_THEME:
.659 #freq
.125 #dura
.0 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.523 #freq
.125 #dura
.0 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.784 #freq
.125 #dura
.375 # wait
.0 # alignment

.392 #freq
.125 #dura
.375 # wait
.0 # alignment

.523 #freq
.125 #dura
.250 # wait
.0 # alignment

.392 #freq
.125 #dura
.250 # wait
.0 # alignment

.330 #freq
.125 #dura
.250 # wait
.0 # alignment

.440 #freq
.125 #dura
.125 # wait
.0 # alignment

.494 #freq
.125 #dura
.125 # wait
.0 # alignment

.466 #freq
.125 #dura
.42 # wait
.0 # alignment

.440 #freq
.125 #dura
.125 # wait
.0 # alignment

.392 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.784 #freq
.125 #dura
.125 # wait
.0 # alignment

.880 #freq
.125 #dura
.125 # wait
.0 # alignment

.698 #freq
.125 #dura
.0 # wait
.0 # alignment

.784 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.587 #freq
.125 #dura
.0 # wait
.0 # alignment

.494 #freq
.125 #dura
.125 # wait
.0 # alignment

.523 #freq
.125 #dura
.250 # wait
.0 # alignment

.392 #freq
.125 #dura
.250 # wait
.0 # alignment

.330 #freq
.125 #dura
.250 # wait
.0 # alignment

.440 #freq
.125 #dura
.125 # wait
.0 # alignment

.494 #freq
.125 #dura
.125 # wait
.0 # alignment

.466 #freq
.125 #dura
.42 # wait
.0 # alignment

.440 #freq
.125 #dura
.125 # wait
.0 # alignment

.392 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.784 #freq
.125 #dura
.125 # wait
.0 # alignment

.880 #freq
.125 #dura
.125 # wait
.0 # alignment

.698 #freq
.125 #dura
.0 # wait
.0 # alignment

.784 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.125 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.587 #freq
.125 #dura
.0 # wait
.0 # alignment

.494 #freq
.125 #dura
.375 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.415 #freq
.125 #dura
.0 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.0 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.1032 #freq
.125 #dura
.125 # wait
.0 # alignment

.1032 #freq
.125 #dura
.0 # wait
.0 # alignment

.1032 #freq
.125 #dura
.625 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.415 #freq
.125 #dura
.0 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.0 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.622 #freq
.125 #dura
.250 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.523 #freq
.125 #dura
.1125 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.415 #freq
.125 #dura
.0 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.0 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.1032 #freq
.125 #dura
.125 # wait
.0 # alignment

.1032 #freq
.125 #dura
.0 # wait
.0 # alignment

.1032 #freq
.125 #dura
.625 # wait
.0 # alignment

.784 #freq
.125 #dura
.0 # wait
.0 # alignment

.740 #freq
.125 #dura
.0 # wait
.0 # alignment

.698 #freq
.125 #dura
.42 # wait
.0 # alignment

.622 #freq
.125 #dura
.125 # wait
.0 # alignment

.659 #freq
.125 #dura
.167 # wait
.0 # alignment

.415 #freq
.125 #dura
.0 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.125 # wait
.0 # alignment

.440 #freq
.125 #dura
.0 # wait
.0 # alignment

.523 #freq
.125 #dura
.0 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.622 #freq
.125 #dura
.250 # wait
.0 # alignment

.587 #freq
.125 #dura
.250 # wait
.0 # alignment

.523 #freq
.125 #dura
.625 # wait
.0 # alignment

# section ending
.0 #freq
.0 #dura
.0 # wait
.0 # alignment
