/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "memu.h"
#include "reg.h"

static const i32_t reg_cpr_reset[NUMBER_OF_REG_CPRS] = {
    [REG_CPR_STATUS]    = 0b00010000010000000000000000000100,
    [REG_CPR_CAUSE] = 0b00000000000000000000000000000000,
    [REG_CPR_PRID]  = 0x00018005,
    [REG_CPR_CONFIG]    = 0b10100100000000000000000010000010,
    [REG_CPR_CONFIG1]   = 0b00011110000000000000000000001010,
    [REG_CPR_WATCHLO]   = 0b00000000000000000000000000000000,
    [REG_CPR_WATCHHI]   = 0b00000000000000000000000000000000,
};

static const i32_t reg_cpr_read_mask[NUMBER_OF_REG_CPRS] = {
    [REG_CPR_STATUS]    = 0b11111111111110001111111111111111,
    [REG_CPR_CAUSE] = 0b11111100111000001111111101111100,
    [REG_CPR_PRID]  = 0b11111111111111111111111111111111,
    [REG_CPR_CONFIG]    = 0b11111111111111111111111110000111,
    [REG_CPR_CONFIG1]   = 0b11111111111111111111111110011111,
    [REG_CPR_WATCHLO]   = 0b11111111111111111111111111111111,
    [REG_CPR_WATCHHI]   = 0b11000000111111110000111111111111,
};

static const i32_t reg_cpr_write_mask[NUMBER_OF_REG_CPRS] = {
    [REG_CPR_STATUS]    = 0b11111110011110001111111100011111,
    [REG_CPR_CAUSE] = 0b00001000110000000000001100000000,
    [REG_CPR_PRID]  = 0b00000000000000000000000000000000,
    [REG_CPR_CONFIG]    = 0b00000000000010000000000000000111,
    [REG_CPR_CONFIG1]   = 0b00000001111111111111111110000000,
    [REG_CPR_WATCHLO]   = 0b11111111111111111111111111111111,
    [REG_CPR_WATCHHI]   = 0b01000000111111110000111111111111,
};

static i32_t reg_gpr[NUMBER_OF_REG_GPRS];
static i32_t reg_gpr_shadow[NUMBER_OF_REG_GPRS];
static i32_t reg_cpr[NUMBER_OF_REG_CPRS];
static i32_t reg_special[NUMBER_OF_REG_SPECIALS];

i32_t reg_init()
{
    memcpy(reg_cpr, reg_cpr_reset, sizeof(reg_cpr));
    return MEMU_SUCCESS;
}

i32_t reg_gpr_read(unsigned int reg, i32_t *word)
{
    //if (reg >= NUMBER_OF_REG_GPRS)
    //    return MEMU_FAILURE;
    reg %= NUMBER_OF_REG_GPRS;
    *word = reg_gpr[reg];
    return MEMU_SUCCESS;
}

i32_t reg_gpr_write(unsigned int reg, i32_t word)
{
    //if (reg >= NUMBER_OF_REG_GPRS)
    //    return MEMU_FAILURE;
    reg %= NUMBER_OF_REG_GPRS;
    reg_gpr[reg] = word;
    reg_gpr[REG_GPR_ZERO] = 0;
    return MEMU_SUCCESS;
}

i32_t reg_gpr_shadow_read(unsigned int rd, unsigned int rt)
{
    rd %= NUMBER_OF_REG_GPRS;
    rt %= NUMBER_OF_REG_GPRS;
    reg_gpr[rd] = reg_gpr_shadow[rt];
#if DUMP_REG_SHADOW
    fprintf(LOG_FILE, "Reg: Shadow: Read: gpr[%d]<-shadow[%d] 0x%.8X\n\n", 
        rd, rt, reg_gpr_shadow[rt]);
#endif
    return MEMU_SUCCESS;
}

i32_t reg_gpr_shadow_write(unsigned int rd, unsigned int rt)
{
    rd %= NUMBER_OF_REG_GPRS;
    rt %= NUMBER_OF_REG_GPRS;
    reg_gpr_shadow[rd] = reg_gpr[rt];
#if DUMP_REG_SHADOW
    fprintf(LOG_FILE, "Reg: Shadow: Write: shadow[%d]<-gpr[%d] 0x%.8X\n\n", 
        rd, rt, reg_gpr[rt]);
#endif
    return MEMU_SUCCESS;
}

i32_t reg_cpr_read(unsigned int reg, unsigned int sel, i32_t *word)
{
    int regsel = (reg << 3) + sel;
    //if (regsel >= NUMBER_OF_REG_CPRS)
    //    return MEMU_FAILURE;
    regsel %= NUMBER_OF_REG_CPRS;

    *word = reg_cpr[regsel];
#if DUMP_REG_CPR
    fprintf(LOG_FILE, "Reg: CPR: Read: reg=%d, sel=%d, word=%.8X\n", reg, sel, *word);
#endif
    return MEMU_SUCCESS;

    switch (regsel)
    {
    case REG_CPR_STATUS:
    case REG_CPR_CAUSE:
    case REG_CPR_PRID:
    case REG_CPR_CONFIG:
    case REG_CPR_CONFIG1:
    case REG_CPR_WATCHLO:
    case REG_CPR_WATCHHI:
        break;
    default:
#if DUMP_REG_CPR
        fprintf(LOG_FILE, "Error: access to CPR not implemented: reg %d, sel %d\n", reg, sel);
#endif
        exit(0);
        break;
    }
    *word = reg_cpr[regsel] & reg_cpr_read_mask[regsel];
#if DUMP_REG_CPR
    fprintf(LOG_FILE, "Reg: CPR: Read: reg=%d, sel=%d, word=%.8X\n", reg, sel, *word);
#endif
    return MEMU_SUCCESS;
}

i32_t reg_cpr_write(unsigned int reg, unsigned int sel, i32_t word)
{
    int regsel = (reg << 3) + sel;
    //if (regsel >= NUMBER_OF_REG_CPRS)
    //    return MEMU_FAILURE;
    regsel %= NUMBER_OF_REG_CPRS;
    
    reg_cpr[regsel] = word;
#if DUMP_REG_CPR
    fprintf(LOG_FILE, "Reg: CPR: Write: reg=%d, sel=%d, word=%.8X\n", reg, sel, word);
#endif
    return MEMU_SUCCESS;

    switch (regsel)
    {
    case REG_CPR_STATUS:
    case REG_CPR_CAUSE:
    case REG_CPR_PRID:
    case REG_CPR_CONFIG:
    case REG_CPR_CONFIG1:
    case REG_CPR_WATCHLO:
    case REG_CPR_WATCHHI:
        break;
    default:
        break;
#if DUMP_REG_CPR
        fprintf(LOG_FILE, "Error: access to CPR not implemented: reg %d, sel %d\n", reg, sel);
        fprintf(LOG_FILE, " -- Wdata: %.8X\n", word);
#endif
        exit(0);
        break;
    }
    reg_cpr[regsel] &= ~reg_cpr_write_mask[regsel];
    reg_cpr[regsel] |= word & reg_cpr_write_mask[regsel];
#if DUMP_REG_CPR
    fprintf(LOG_FILE, "Reg: CPR: Write: reg=%d, sel=%d, word=%.8X\n", reg, sel, word);
#endif
    return MEMU_SUCCESS;
}


i32_t reg_special_read(unsigned int reg, i32_t *word)
{
    //if (reg >= NUMBER_OF_REG_SPECIALS)
    //    return MEMU_FAILURE;
    reg %= NUMBER_OF_REG_SPECIALS;
    *word = reg_special[reg];
    return MEMU_SUCCESS;
}

i32_t reg_special_write(unsigned int reg, i32_t word)	
{
    //if (reg >= NUMBER_OF_REG_SPECIALS)
    //    return MEMU_FAILURE;
    reg %= NUMBER_OF_REG_SPECIALS;
    reg_special[reg] = word;
    return MEMU_SUCCESS;
}

i32_t reg_advance_pc()
{
    reg_special[REG_SPECIAL_PC_PREV] = reg_special[REG_SPECIAL_PC];
    reg_special[REG_SPECIAL_PC] = reg_special[REG_SPECIAL_PC_ADVANCE1];
    reg_special[REG_SPECIAL_PC_ADVANCE1] = reg_special[REG_SPECIAL_PC_ADVANCE2];
    reg_special[REG_SPECIAL_PC_ADVANCE2] += 4;
    return reg_special[REG_SPECIAL_PC];
}

