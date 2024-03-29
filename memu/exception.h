/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

enum _exceptions 
{
    EXCEPTION_NONE,
    EXCEPTION_RESET,
    EXCEPTION_SOFT_RESET,
    EXCEPTION_NONMASKABLE_INTERRUPT,
    EXCEPTION_MACHINE_CHECK,
    EXCEPTION_ADDRESS_ERROR,
    EXCEPTION_TLB_REFILL,
    EXCEPTION_EXECUTE_INHIBIT,
    EXCEPTION_READ_INHIBIT,
    EXCEPTION_TLB_INVALID,
    EXCEPTION_TLB_MODIFIED,
    EXCEPTION_CACHE_ERROR,
    EXCEPTION_BUS_ERROR,
    EXCEPTION_INTEGER_OVERFLOW,
    EXCEPTION_TRAP,
    EXCEPTION_SYSTEM_CALL,
    EXCEPTION_BREAKPOINT,
    EXCEPTION_RESERVED_INSTRUCTION,
    EXCEPTION_COPROCESSOR_UNUSABLE,
    EXCEPTION_FLOATING_POINT,
    EXCEPTION_COPROCESSOR_2,
    EXCEPTION_WATCH,
    EXCEPTION_INTERRUPT,
    NUMBER_OF_EXCEPTIONS,
};

#endif /* EXCEPTION_H */
