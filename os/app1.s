.inc "jmp_fix.inc"
.inc "io.lib"

BEG:
    sw $sp, 100($zero)
    ori $a0, $zero, 97
    jal PUTC
    #wait
    j BEG
END:
    j END

    