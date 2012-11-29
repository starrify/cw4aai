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
static i32_t mmud_backup;

int mmu_init()
{
    get_dma_info(&membase, &memsize);
    reg_cpr_write(FKREG_CPR_MMUD, 0, 1);
    return MEMU_SUCCESS;
}

int mmu_disable()
{
    reg_cpr_read(FKREG_CPR_MMUD, 0, &mmud_backup);
    reg_cpr_write(FKREG_CPR_MMUD, 0, 1);
#if DUMP_MMU
    fprintf(LOG_FILE, "MMU: disabled. saved previous MMUD state: %d\n", mmud_backup);
#endif
    return MEMU_SUCCESS;
}

int mmu_restore()
{
    i32_t mmud;
    reg_cpr_read(FKREG_CPR_MMUD, 0, &mmud);
    reg_cpr_write(FKREG_CPR_MMUD, 0, mmud_backup);
#if DUMP_MMU
    fprintf(LOG_FILE, "MMU: MMUD restored to %d. ignored previous state: %d\n", mmud_backup, mmud);
#endif
    return MEMU_SUCCESS;
}

int mmu_addr_trans(u32_t vaddr, int access_type, u32_t *paddr, u32_t *attr)
{  
    u32_t gdtbase;
    u32_t mmud;
    reg_cpr_read(FKREG_CPR_GDTBASE, 0, &gdtbase);
    reg_cpr_read(FKREG_CPR_MMUD, 0, &mmud);
    int ret = EXCEPTION_NONE;
    if (!gdtbase || mmud)
    {
        *paddr = vaddr;
    }
    else if (gdtbase >= config.memsize)
    {
#if DUMP_MMU
        fprintf(LOG_FILE, "MMU: error: memory size limit exceeded: gdtbase=0x%.8X\n\n", gdtbase);
#endif
    }
    else
    {
        u32_t *pgdt = membase + gdtbase;
        int i;
        for (i = 0; i < GDT_SIZE; i++)
        {
            // in a gdt entry: paddr/vaddr/sz/attr
            if (pgdt[1] <= vaddr && vaddr < pgdt[1] + pgdt[2] && pgdt[3])
            {
                *paddr = vaddr + pgdt[0] - pgdt[1];
                break;
            }
            pgdt += 4;
        }
        if (i == GDT_SIZE) // entry match failed
        {
            reg_cpr_write(FKREG_CPR_PFLA, 0, vaddr);
            //ret = 
            *paddr = vaddr;
#if DUMP_MMU
            fprintf(LOG_FILE, "MMU: error: failed to match an entry in GDT\n");
#endif
        }
    }
#if DUMP_MMU
    fprintf(LOG_FILE, "MMU: mmud=%d, gdtbase=0x%.8X, vaddr=0x%.8X, paddr=0x%.8X, access_type=0x%X\n", 
        mmud, gdtbase, vaddr, *paddr, access_type);
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

