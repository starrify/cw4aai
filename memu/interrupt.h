/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stddef.h>
#include "memu.h"

#define INTERRUPT_ENTRY_KEYBOARD_INPUT  0x00000080
#define INTERRUPT_ENTRY_TIMER   0x00000100
#define INTERRUPT_ENTRY_SYSCALL 0x00000180

extern int interrupt(u32_t entry);

#endif /* INTERRUPT_H */
