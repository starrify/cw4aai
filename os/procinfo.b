
    #32*4word: gdt, 64word: reserved:
    #32word: gpr, 1word: epc, 1word: hi, 1word: lo, 1word: status, 28word: reserved
    #256words in total
    #proc_info for init
#0x01000000 = PROC_USRSPACE_SIZE, however, macro is not used
#because calculation in .loop is not supported in preproc yet
.loop 0 0xfffffff 0x00800000 {
    .<LOOPVAR>  #physical: <LOOPVAR>
    .0x0        #virtual: 0
    .0x800000   #size: 8M
    .1          #valid
    .0x0        #physical: 0
    .0x1800000  #virtual: 24M
    .0x800000   #size: 8M
    .1          #valid
    .dup 248 .0
}
