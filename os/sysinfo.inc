.inc "var.inc"
.def SETFRM_SYSINFO {SETFRM 32768}

#sysinfo
.def SCR_BASE {48}
.def SCR_OFFSET {52}
.def CURSOR {56}
.def INBUF_BASE {64}
.def INBUF_START {72}
.def INBUF_END {76}

#const
.def INBUF_SIZE {4096}
.def INBUF_UNIT_SIZE {4}
.def SCR_WIDTH {80}     #maybe should read from sysinfo?
.def SCR_UNIT_SIZE {4}
.def SCR_HEIGHT {35}
.def OS_STACK_INIT {0x00800000}
