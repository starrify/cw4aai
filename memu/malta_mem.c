/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "exception.h"
#include "malta_mem.h"
#include "memu.h"
#include "gt64120.h"

static void *membase;
static size_t memsize;

static int malta_mem_readflash(char *filename)
{
    FILE *fl = fopen(filename, "r");
    if (!fl)
        return MEMU_FAILURE;
    char str[0x40];
    int paddr = 0;
    int tmp;
    while (fgets(str, 0x40, fl))
    {
        switch (*str)
        {
        case '#':
        case '!':
        case '>':
            break;
        case '@':
            paddr = strtol(str + 1, NULL, 0x10);
            break;
        default:
            if (paddr >= memsize)
                return MEMU_FAILURE;
            tmp = strtol(str, NULL, 0x10);
            *(int*)(membase + paddr) = tmp;
            paddr += sizeof(int); 
            break;
        }
    }
    fclose(fl);
    return MEMU_SUCCESS;
}

void get_dma_info(void **_membase, size_t *_memsize)
{
    *_membase = membase;
    *_memsize = memsize;
    return;
}

int malta_mem_create(size_t size)
{
    membase = malloc(size);
    if (!membase)
        return MEMU_FAILURE;
    memsize = size;
    return MEMU_SUCCESS;
}

int malta_mem_init()
{
    int ret = 0;
    
    malta_mem_readflash(config.flash_file);
    
    /* Revision register of Malta board */
    *(u32_t*)(membase + 0x1FC00010) = 0x00000420;
    
    /* the GT-64120 system controller */
    gt64120_reg_init(0x00A0);
    
    /* the IIC bus */
    iic_init();
    
    return ret;
}

int malta_mem_destroy()
{
    free(membase);
    return MEMU_SUCCESS;
}

int malta_mem_read(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t *word)
{
    int len = MEM_ACCESS_LEN(access_type);
    //assert(!(access_type & MEM_ACCESS_WRITE));
    if (paddr > memsize - len)
        return EXCEPTION_ADDRESS_ERROR;
    switch (len)
    {
    case 4:
        *word = (i32_t)*(i32_t*)(paddr + membase);
        break;
    case 2:
        *word = (i32_t)*(i16_t*)(paddr + membase);
        break;
    case 1:
        *word = (i32_t)*(i8_t*)(paddr + membase);
        break;
    default:
        assert(0);
        break;
    }

#if DUMP_MEM
    if (access_type & MEM_ACCESS_DATA)
        fprintf(LOG_FILE, "MEM: Read: paddr=%.8X, len=%d, word=%.8X\n", paddr, len, *word);
#endif
    return EXCEPTION_NONE;
}

int malta_mem_write(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t word)
{
    int len = MEM_ACCESS_LEN(access_type);
    //assert(!(access_type & MEM_ACCESS_READ));
    if (paddr > memsize - len)
        return EXCEPTION_ADDRESS_ERROR;
    switch (len)
    {
    case 4:
        *(i32_t*)(paddr + membase) = (i32_t)word;
        if (paddr == addr_gt64120_reg_isd)
            gt64120_reg_init(word);
        //*(u32_t*)(membase + 0x1F000B00) = *(u32_t*)(membase + 0x1F000B08) & *(u32_t*)(membase + 0x1F000B10);
        break;
    case 2:
        *(i16_t*)(paddr + membase) = (i16_t)word;
        break;
    case 1:
        *(i8_t*)(paddr + membase) = (i8_t)word;
        break;
    default:
        assert(0);
        break;
    }

#if DUMP_MEM
    if (access_type & MEM_ACCESS_DATA)
       fprintf(LOG_FILE, "MEM: Write: paddr=%.8X, len=%d, word=%.8X\n", paddr, len, word);
#endif
    return EXCEPTION_NONE;
}

