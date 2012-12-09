.inc "../inc/var.inc"
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
.def HDD_SECT_LOG_SIZE {9}
.def HDD_SECT_SIZE {(2 << HDD_SECT_LOG_SIZE)}
.def DMA_ENABLE {160}
.def DMA_DST_ADDR {164}
.def DMA_SRC_ADDR {168}
.def DMA_SIZE {172}

#const
.def INBUF_SIZE {4096}
.def INBUF_UNIT_SIZE {4}
.def SCR_WIDTH {80}     #maybe should read from sysinfo?
.def SCR_UNIT_SIZE {4}
.def SCR_HEIGHT {35}
.def SYS_STACK_INIT {0x01C80000}
