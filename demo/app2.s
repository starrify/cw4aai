.inc "jmp_fix.inc"
.inc "io.lib"

BEG:
    ori $a0, $zero, 98
    jal PUTC
    wait
    j BEG
END:
    j END
