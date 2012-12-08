.inc "../inc/jmp_fix.inc"
.inc "../inc/pseudo_inst.inc"
.inc "../inc/syscall.inc"
.inc "interrupt.lib"
.inc "sysinfo.inc"

.offset 0x1c00000 #28M

BOOT:
    #calc physical address for proc_info
    lli $t0, 0x1800000
    lla $t1, PROC_INFO
    sub $t1, $t1, $t0
    #set gdt position
    mtc0 $t1, $0
    #enable mmu
    mtc0 $zero, $4
    
    j SYS_INIT
    
SYS_INIT:
    #init os stack
    #when running init code, treat system as an ordinary process
    lli $sp, PROC_USRSPACE_SIZE
    #init interrupt
    jal INT_INIT
    #init libs
    #interrupt enable
    mtc0 $t0, $3

SYS_MAIN:
    ori $k0, $zero, SC_FORK
    syscall
    bne $v0, $zero, SYS_END
SYS_MAIN_CLD:
    ori $k0, $zero, SC_EXEC
    ori $a0, $zero, 1   #supposed to be shell
    syscall
    
SYS_MAIN_CLDERR:
    .4  #errcode
    j SYS_MAIN_CLDERR
    
SYS_END:
    #suspend proc 0, may be resumed by proc_schedule
    SETFRMLB PROC_INFO
    ori $t0, $zero, PROC_SUSPENDED
    sw $t0, PROC_STATE_OFF_PER_PROC($fp)
    wait
    j SYS_END
