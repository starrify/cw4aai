/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#include "gt64120.h"
#include "memu.h"
#include "malta_mem.h"

u32_t addr_gt64120_reg_isd;

int gt64120_reg_init(u32_t gt64120_reg_isd)
{
    u32_t gt64120_reg_base = gt64120_reg_isd << 21;
    
    void *membase;
    size_t memsize;
    get_dma_info(&membase, &memsize);
    
    /* CPU Configuration */
    *(u32_t*)(membase + gt64120_reg_base + 0x000) = 0b00000000000000000001000000000000;
    
    /* Internal Space Decode */
    *(u32_t*)(membase + gt64120_reg_base + 0x068) = gt64120_reg_isd;
    
    addr_gt64120_reg_isd = gt64120_reg_base + 0x68;
    
    return MEMU_SUCCESS;
}





















