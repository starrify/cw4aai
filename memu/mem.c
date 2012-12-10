/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interrupt.h"
#include "config.h"
#include "exception.h"
#include "memu.h"
#include "mem.h"

static void *membase;
static size_t memsize;
static u32_t *sbase;

#if 0
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
#endif

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
    *(u32_t*)(membase + 0) = 0x42000018;    //ERET the empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_KEYBOARD_INPUT) = 0x00000000;    //ERET as an empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_TIMER) = 0x00000000;    //ERET as an empty exception handler
    *(u32_t*)(membase + INTERRUPT_ENTRY_SYSCALL) = 0x00000000;    //ERET as an empty exception handler

    /* sysinfo and device mapping 0x00008000-0x007FFFFF */
    sbase = membase + config.sbase_offset;
    //*(sbase + 0) = 0;  //segment offset // moved to register
    *(sbase + 8) = 0;  //char mode
    *(sbase + 9) = 35; //screen x
    *(sbase + 10) = 80; //screen y
    *(sbase + 12) = 0x00040000; //framebuffer base
    *(sbase + 13) = 0;  //screen start offsite in framebuffer base
    *(sbase + 14) = 0;  //cursor_off
    *(sbase + 16) = 0x00020000; //input buffer base
    *(sbase + 17) = 0x00001000; //input buffer size
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

int mem_write(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t word)
{
    int len = MEM_ACCESS_LEN(access_type);
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
    if (paddr == config.sbase_offset + 0x00000080)  // HDD access. see spec.txt
    {
        u32_t type = *(sbase + 0x00000020);
        u32_t memstart = *(sbase + 0x00000021);
        u32_t secstart = *(sbase + 0x00000022);
        u32_t seccnt = *(sbase + 0x00000023);
        if (type == 1) // read
        {
            hdd_read(memstart, secstart, seccnt);
        }
        else if (type == 2) // write
        {
            hdd_write(memstart, secstart, seccnt);
        }
        else
        {
#if DUMP_HDD
            fprintf(LOG_FILE, "HDD: unknown type=%d, memstart=0x%.8X, secstart=0x%.8X, seccnt=0x%.8X\n",
                type, memstart, secstart, seccnt);
#endif
        }
    }
    else if (paddr == config.sbase_offset + 0x000000A0) // DMA copy
    {
        u32_t type = *(sbase + 0x00000028);
        u32_t dst = *(sbase + 0x00000029);
        u32_t src = *(sbase + 0x0000002A);
        u32_t sz = *(sbase + 0x0000002B);
#if DUMP_DMA
        fprintf(LOG_FILE, "DMA: type=%d, dst=0x%.8X, src=0x%.8X, sz=0x%.8X\n", type, dst, src, sz);
#endif
        if (type == 1) // memcpy
        {
            memcpy(membase + dst, membase + src, sz);
        }
        else //if (type == 2) // memset?
        {
        }
    }
    else if (paddr == config.sbase_offset + 0x000000C0) // PC Speaker
    {
        u32_t type = *(sbase + 0x00000030);
        u32_t freq = *(sbase + 0x00000031);
        u32_t dura = *(sbase + 0x00000032);
        char *s;
        asprintf(&s,  "beep -f %d -l %d", freq, dura);
#if DUMP_SPEAKER
        fprintf(LOG_FILE, "Speaker: %s\n", s);
#endif
        if (fork() == 0)
        {
            system(s);
            exit(0);
        }
        free(s);
    }
    
#if DUMP_MEM
    if (access_type & MEM_ACCESS_DATA)
       fprintf(LOG_FILE, "MEM: Write: paddr=%.8X, len=%d, word=%.8X\n", paddr, len, word);
#endif
    return EXCEPTION_NONE;
}

