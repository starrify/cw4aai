/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef IIC_H
#define IIC_H

#include "memu.h"

#define I2CSCL  2
#define I2CSDA  1

extern const u32_t i2cinp_addr;
extern const u32_t i2coe_addr;
extern const u32_t i2cout_addr;
extern const u32_t i2csel_addr;

extern int iic_init();
extern void *iic_daemon(void *ptr);

#endif /* IIC_H */
