/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include "reg.h"
#include "exception.h"
#include "interrupt.h"

int interrupt_init()
{
    return MEMU_SUCCESS;
}


int interrupt(u32_t entry)
{
    u32_t pcdata;
    reg_special_read(REG_SPECIAL_PC, &pcdata);

    return EXCEPTION_NONE;

}
