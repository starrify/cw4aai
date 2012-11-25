/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef HDD_H
#define HDD_H

#include <stddef.h>
#include "memu.h"

#define SECTOR_SIZE 0x200

extern int hdd_init();
extern int hdd_read(void *mem_start, size_t sector_start, size_t sector_count);
extern int hdd_write(void *mem_start, size_t sector_start, size_t sector_count);

#endif /* MEM_H */
