.inc "pseudo_inst.inc"
.def STKVAR_S r1, var {sw r1, var($sp)}
.def STKVAR_L r1, var {lw r1, var($sp)}

.def SETFRM pos {ori $fp, $zero, pos}
.def SETFRML pos {lil $fp, $zero, pos}
.def SETFRM_SYSINFO {SETFRM 32768}
.def SETFRM_STATIC {SETFRML STATIC_HERE}

#sysinfo
.def SCR_BASE {48}
.def SCR_OFFSET {52}
.def CURSOR {56}
.def INBUF_BASE {64}
.def INBUF_START {72}
.def INBUF_END {76}

#os var
#.def LIB_INBUF_BASE {0x1000}
#.def LIB_INBUF_START {0x1004}
#.def LIB_INBUF_END {0x1008}

#const
.def INBUF_SIZE {65536}
.def SCR_WIDTH {80}
.def SCR_HEIGHT {35}
.def OS_STACK_INIT {0x8000}

#.def LIB_INBUF_SIZE {4096}
#.def LIB_INBUF_BASE_VALUE {0x800000} #0x800000
