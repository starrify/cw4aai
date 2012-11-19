/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include "memu.h"

enum _mem_access_type
{
    MEM_ACCESS_DATA = 1 << 0,
    MEM_ACCESS_INST = 1 << 1,
    MEM_ACCESS_READ = 1 << 2,
    MEM_ACCESS_WRITE    = 1 << 3,
    MEM_ACCESS_LEN_1    = 1 << 4,
    MEM_ACCESS_LEN_2    = 2 << 4,
    MEM_ACCESS_LEN_4    = 4 << 4,
};

#define MEM_ACCESS_LEN(type)    MASKSHR(type, 6, 4)

extern void get_dma_info(void **membase, size_t *memsize);
extern int mem_create(size_t size);
extern int mem_init();
extern int mem_loadimg(char const *filename, u32_t img_base);
extern int mem_destroy();
extern int mem_read(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t *word);
extern int mem_write(u32_t paddr, u32_t vaddr, u32_t attr, int access_type, i32_t word);

#endif /* MEM_H */
