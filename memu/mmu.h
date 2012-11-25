/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef MMU_H
#define MMU_H

#include "memu.h"

extern int mmu_init();
extern int mmu_addr_trans(u32_t vaddr, int access_type, u32_t *paddr, u32_t *attr);

#endif /* MMU_H */
