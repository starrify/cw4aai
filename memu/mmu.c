/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include "reg.h"
#include "mmu.h"
#include "malta_mem.h"
#include "memu.h"
#include "exception.h"
#include "config.h"

static void *membase;
static size_t memsize;

int mmu_init()
{
    get_dma_info(&membase, &memsize);
    return MEMU_SUCCESS;
}

int mmu_addr_trans(u32_t vaddr, int access_type, u32_t *paddr, u32_t *attr)
{  
    u32_t gdtbase;
    reg_cpr_read(FKREG_CPR_GDTBASE, 0, &gdtbase);
    if (!gdtbase)
    {
        *paddr = vaddr;
    }
    else
    {
        u32_t *pgdt = membase + gdtbase;
        int i;
        for (i = 0; i < 2; i++)
        {
            // in a gdt entry: paddr/vaddr/sz/attr
            if (pgdt[1] <= vaddr && vaddr < pgdt[1] + pgdt[2])
            {
                *paddr = vaddr + pgdt[0] - pgdt[1];
                break;
            }
        }
        if (i == 2) // entry match failed
        {
            *paddr = vaddr;
#if DUMP_MMU
            fprintf(LOG_FILE, "MMU: error: failed to match an entry in GDT\n");
#endif
        }
    }
#if DUMP_MMU
    fprintf(LOG_FILE, "MMU: gdtbase=0x%.8X, vaddr=0x%.8X, paddr=0x%.8X, access_type=0x%X\n", 
        gdtbase, vaddr, *paddr, access_type);
#endif
     return EXCEPTION_NONE;
    /*
    // only kernel mode implemented for now 
    switch (MASKSHR(vaddr, 31, 29))
    {
    case 0b111: // kseg3: 0xE0000000 to 0xFFFFFFFF 
        return EXCEPTION_ADDRESS_ERROR;
        break;
    case 0b110: // ksseg: 0xC0000000 to 0xDFFFFFFF 
        return EXCEPTION_ADDRESS_ERROR;
        break;
    case 0b101: // kseg1: 0xA0000000 to 0xBFFFFFFF 
        *paddr = MASK(vaddr, 28, 0);
        break;
    case 0b100: // kseg0: 0x80000000 to 0x9FFFFFFF 
        *paddr = MASK(vaddr, 28, 0);
        break;
    case 0b011: 
    case 0b010:
    case 0b001:
    case 0b000: // useg: 0x00000000 to 0x7FFFFFFFF 
        return EXCEPTION_ADDRESS_ERROR;
        break;
    default:
        // execution flow shouldn't reach here 
        break;    
    }
    return EXCEPTION_NONE;
    */
}

static int addr_trans(u32_t addr, u32_t *phyaddr)
{
    *phyaddr = addr;
    return EXCEPTION_NONE;

    /* only kernel mode implemented for now */
    switch (MASKSHR(addr, 31, 29))
    {
    case 0b111: /* kseg3: 0xE0000000 to 0xFFFFFFFF */
        return EXCEPTION_ADDRESS_ERROR;
        break;
    case 0b110: /* ksseg: 0xC0000000 to 0xDFFFFFFF */
        return EXCEPTION_ADDRESS_ERROR;
        break;
    case 0b101: /* kseg1: 0xA0000000 to 0xBFFFFFFF */
        *phyaddr = MASK(addr, 28, 0);
        break;
    case 0b100: /* kseg0: 0x80000000 to 0x9FFFFFFF */
        *phyaddr = MASK(addr, 28, 0);
        break;
    case 0b011: 
    case 0b010:
    case 0b001:
    case 0b000: /* useg: 0x00000000 to 0x7FFFFFFFF */
        return EXCEPTION_ADDRESS_ERROR;
        break;
    default:
        /* execution flow shouldn't reach here */
        break;    
    }
    return EXCEPTION_NONE;
}

