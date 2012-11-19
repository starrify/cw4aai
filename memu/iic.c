/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "iic.h"
#include "memu.h"
#include "config.h"
#include "malta_mem.h"

const u32_t iicinp_addr = 0x1F000B00;
const u32_t iicoe_addr  = 0x1F000B08;
const u32_t iicout_addr = 0x1F000B10;
const u32_t iicsel_addr = 0x1F000B18;

int iic_init()
{
    void *membase;
    size_t memsize;
    get_dma_info(&membase, &memsize);
    
    *(u32_t*)(membase + iicinp_addr) = 0x00000003;
    *(u32_t*)(membase + iicoe_addr)  = 0x00000000;
    *(u32_t*)(membase + iicout_addr) = 0x00000003;
    *(u32_t*)(membase + iicsel_addr) = 0x00000001;        
    
    return MEMU_SUCCESS;
}

void *iic_daemon(void *ptr)
{
    void *membase;
    size_t memsize;
    get_dma_info(&membase, &memsize);
    
    while (1)
    {
        struct timespec rqtp = { .tv_sec = 0, .tv_nsec = config.iic_check_rate };
        nanosleep(&rqtp, NULL);
        
        const u32_t fresh_data = (1u << 2);
        
        static int state = 0; /* idle */
        if (fresh_data & *(u32_t*)(membase + iicout_addr))
        {
        
        
        }
#if DUMP_IIC
    fprintf(LOG_FILE, "IIC: test dump\n");
#endif
               
    }
    return NULL;
}
