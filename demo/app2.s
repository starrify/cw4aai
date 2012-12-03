.inc "../inc/jmp_fix.inc"
.inc "../lib/io.lib"

BEG:
    ori $a0, $zero, 98
    jal PUTC
    wait
    j BEG
END:
    j END
