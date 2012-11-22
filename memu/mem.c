/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "interrupt.h"
#include "config.h"
#include "exception.h"
#include "memu.h"
#include "mem.h"

static void *membase;
static size_t memsize;

int mem_loadimg(char const *filename, u32_t offset)
{
    FILE *fin = fopen(filename, "rb");
    if (!fin)
    {
        fprintf(stderr, "Error loading image %s\n", filename);
        exit(1);
    }
    fseek(fin, 0, SEEK_END);
    int fsz = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    fread(membase + offset, fsz, 1, fin);
    fclose(fin);
    return MEMU_SUCCESS;
}

void get_dma_info(void **_membase, size_t *_memsize)
{
    *_membase = membase;
    *_memsize = memsize;
    return;
}

int mem_create(size_t size)
{
    membase = malloc(size);
    if (!membase)
        return MEMU_FAILURE;
    memsize = size;
    return MEMU_SUCCESS;
}

int mem_init()
{
    if (!membase)
        return -1;
    int ret = 0;

    /* interrput vector table */
    *(u32_t*)(membase + 0) = 0x42000018;    //ERET as an empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_KEYBOARD_INPUT) = 0x42000018;    //ERET as an empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_TIMER) = 0x42000018;    //ERET as an empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_SYSCALL) = 0x42000018;    //ERET as an empty exception handler

    /* sysinfo and device mapping 0x00008000-0x007FFFFF */
    unsigned int *sbase = membase + config.sbase_offset;
    //*(sbase + 0) = 0;  //segment offset // moved to register
    *(sbase + 8) = 0;  //char mode
    *(sbase + 9) = 35; //screen x
    *(sbase + 10) = 80; //screen y
    *(sbase + 12) = 0x00040000; //framebuffer base
    *(sbase + 13) = 0;  //screen start offsite in framebuffer base
    *(sbase + 14) = 0;  //cursor_off
    *(sbase + 16) = 0x00020000; //input buffer base
    *(sbase + 17) = 0x00010000; //input buffer size
    *(sbase + 18) = 0x00000000; //input queue head
    *(sbase + 19) = 0x00000000; //input queue tail
    *(sbase + 24) = 0;  //seconds since EPOCH
    *(sbase + 25) = 0;  //millisecond counter

    unsigned int fbbase = *(sbase + 12);
    int i;
    for (i = 0; i < 35*80; i++)
        *(unsigned int*)(membase + fbbase + i * 4) = (unsigned int)' ';
        

    return ret;
}

int mem_destroy()
{
    free(membase);
    return MEMU_SUCCESS;
}

int mem_read(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t *word)
{
    int len = MEM_ACCESS_LEN(access_type);
#if DUMP_MEM
    if (access_type & MEM_ACCESS_DATA)
        fprintf(LOG_FILE, "MEM: Read: paddr=%.8X, len=%d, word=%.8X\n", paddr, len, *word);
#endif
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
   return EXCEPTION_NONE;
}

int mem_write(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t word)
{
    int len = MEM_ACCESS_LEN(access_type);
#if DUMP_MEM
    if (access_type & MEM_ACCESS_DATA)
       fprintf(LOG_FILE, "MEM: Write: paddr=%.8X, len=%d, word=%.8X\n", paddr, len, word);
#endif
    //assert(!(access_type & MEM_ACCESS_READ));
    if (paddr > memsize - len)
        return EXCEPTION_ADDRESS_ERROR;
    switch (len)
    {
    case 4:
        *(i32_t*)(paddr + membase) = (i32_t)word;
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
   return EXCEPTION_NONE;
}

