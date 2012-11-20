/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef REG_H
#define REG_H

#include "memu.h"

#define REG_CPR_SEL(reg, sel)   (((reg) << 3) + (sel))

enum _reg_gpr
{
    REG_GPR_ZERO,
    REG_GPR_AT,
    REG_GPR_V0,
    REG_GPR_V1,
    REG_GPR_A0,
    REG_GPR_A1,
    REG_GPR_A2,
    REG_GPR_A3,
    REG_GPR_T0,
    REG_GPR_T1,
    REG_GPR_T2,
    REG_GPR_T3,
    REG_GPR_T4,
    REG_GPR_T5,
    REG_GPR_T6,
    REG_GPR_T7,
    REG_GPR_S0,
    REG_GPR_S1,
    REG_GPR_S2,
    REG_GPR_S3,
    REG_GPR_S4,
    REG_GPR_S5,
    REG_GPR_S6,
    REG_GPR_S7,
    REG_GPR_T8,
    REG_GPR_T9,
    REG_GPR_K0,
    REG_GPR_K1,
    REG_GPR_GP,
    REG_GPR_SP,
    REG_GPR_FP,
    REG_GPR_RA,
    NUMBER_OF_REG_GPRS,
};

enum _reg_special
{
    REG_SPECIAL_BRANCH_JUMP,
    REG_SPECIAL_PC_PREV,
    REG_SPECIAL_PC,
    REG_SPECIAL_PC_ADVANCE1,
    REG_SPECIAL_PC_ADVANCE2,
    REG_SPECIAL_HI,
    REG_SPECIAL_LO,
    NUMBER_OF_REG_SPECIALS,
};

enum _reg_cpr
{
    /* implemented only those required by MIPS 32 Release 1 for now */
    REG_CPR_INDEX   = REG_CPR_SEL(0, 0),
    REG_CPR_RANDOM  = REG_CPR_SEL(1, 0),
    REG_CPR_ENTRYLO0    = REG_CPR_SEL(2, 0),
    REG_CPR_ENTRYLO1    = REG_CPR_SEL(3, 0),
    REG_CPR_CONTEXT = REG_CPR_SEL(4, 0),
    REG_CPR_PAGEMASK    = REG_CPR_SEL(5, 0),
    REG_CPR_WIRED   = REG_CPR_SEL(6, 0),
    REG_CPR_HWRENA  = REG_CPR_SEL(7, 0),
    REG_CPR_BADVADDR    = REG_CPR_SEL(8, 0),
    REG_CPR_COUNT   = REG_CPR_SEL(9, 0),
    REG_CPR_ENTRYHI = REG_CPR_SEL(10, 0),
    REG_CPR_COMPARE = REG_CPR_SEL(11, 0),
    REG_CPR_STATUS  = REG_CPR_SEL(12, 0),
    REG_CPR_CAUSE   = REG_CPR_SEL(13, 0),
    REG_CPR_EPC = REG_CPR_SEL(14, 0),
    REG_CPR_PRID    = REG_CPR_SEL(15, 0),
    REG_CPR_CONFIG  = REG_CPR_SEL(16, 0),
    REG_CPR_CONFIG1 = REG_CPR_SEL(16, 1),
    REG_CPR_WATCHLO = REG_CPR_SEL(18, 0),
    REG_CPR_WATCHHI = REG_CPR_SEL(19, 0),
    REG_CPR_ERROREPC    = REG_CPR_SEL(30, 0),
    NUMBER_OF_REG_CPRS  = 32 * 8, /* 32 registers * 8 selections */
};

enum _fake_reg_cpr
{
    FKREG_CPR_SEGBASE = 0,
    FKREG_CPR_EPC,
    FKREG_CPR_EXL,
    FKREG_CPR_IE,
    FKREG_CPR_RESERVED,
};

extern i32_t reg_init();
extern i32_t reg_gpr_read(unsigned int reg, i32_t *word);
extern i32_t reg_gpr_write(unsigned int reg, i32_t word);
extern i32_t reg_cpr_read(unsigned int reg, unsigned int sel, i32_t *word);
extern i32_t reg_cpr_write(unsigned int reg, unsigned int sel, i32_t word);
extern i32_t reg_special_read(unsigned int reg, i32_t *word);
extern i32_t reg_special_write(unsigned int reg, i32_t word);
extern i32_t reg_advance_pc();

#endif /* REG_H */
