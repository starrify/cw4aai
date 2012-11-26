.inc "var.inc"
#sysinfo in virtual address
.def SETFRM_SYSINFO {SETFRML 0x1808000}

#sysinfo
.def SCR_BASE {48}
.def SCR_OFFSET {52}
.def CURSOR {56}
.def INBUF_BASE {64}
.def INBUF_START {72}
.def INBUF_END {76}
.def HDD_ENABLE {128}
.def HDD_SPMO {132}
.def HDD_SSN {136}
.def HDD_MAX_TSIZE {140}

#const
.def INBUF_SIZE {4096}
.def INBUF_UNIT_SIZE {4}
.def SCR_WIDTH {80}     #maybe should read from sysinfo?
.def SCR_UNIT_SIZE {4}
.def SCR_HEIGHT {35}
.def SYS_STACK_INIT {0x1C800000}
