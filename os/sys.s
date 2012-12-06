.inc "../inc/jmp_fix.inc"
.inc "../inc/pseudo_inst.inc"
.inc "../inc/var.inc"
.inc "../lib/io.lib"
.inc "interrupt.lib"
.inc "sysinfo.inc"
.inc "shell.lib"

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
    #jal IO_INIT
    #interrupt enable
 
    mtc0 $t0, $3

SHELL:
    #entering shell
    #local variable
    .def LOCAL_SHELL_CMD_OFF {0}
    addi $sp, $sp, -4 * 80
SHELLLP:
    lla $a0, SH_PROMPT
    jal PUTS
    addi $a0, $sp, LOCAL_SHELL_CMD_OFF
    jal GETS
    or $a0, $zero, $v0
    jal SH_CMD
    j SHELL

SHELLEND:
    addi $sp, $sp, 4 * 80
END:
    wait
    j END

