.inc "var.inc"
.inc "pseudo_inst.inc"

LIBINIT:
        #initialize lib input base
        lil $t0, LIB_INPUT_BASE_VALUE
        OSVAR_S $t0, LIB_INPUT_BASE
        OSVAR_S $zero, LIB_INPUT_START
        OSVAR_S $zero, LIB_INPUT_END
        
        jr $ra