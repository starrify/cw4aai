.inc "var.inc"
.inc "pseudo_inst.inc"

LIBINIT:
        #initialize lib input base
        lil $t0, LIB_INPUT_BASE_VALUE
        sw $t0, GLBVAR LIB_INPUT_BASE
        sw $zero, GLBVAR LIB_INPUT_START
        sw $zero, GLBVAR LIB_INPUT_END

        jr $ra