    #8word: gdt, 32word: gpr, 1word: epc, 1word: hi, 1word: lo, 21word: reserved
    #proc_info for init
.loop 0 0xfffffff 0x2000000 {
    .<LOOPVAR>  #physical: <LOOPVAR>
    .0x0        #virtual: 0
    .0x1800000  #size: 24M
    .0          #reserved
    .0x0        #physical: 0
    .0x1800000  #virtual: 24M
    .0x800000   #size: 8M
    .0          #reserved
    .dup 56 .0
}
