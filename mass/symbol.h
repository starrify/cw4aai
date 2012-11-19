/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef SYMBOL_H
#define SYMBOL_H

enum LABEL_TYPE_
{
    LABEL_RELATIVE_,
    LABEL_ABSOLUTE_,
    LABEL_16_BIT_,
    LABEL_26_BIT_,
    LABEL_REFERENCE_,
    LABEL_DECLARE_
};

enum SYMBOL_TYPE_
{
    SYMBOL_LABEL
};

#define LABEL_RELATIVE  (1 << LABEL_RELATIVE_)
#define LABEL_ABSOLUTE  (1 << LABEL_ABSOLUTE_)
#define LABEL_16_BIT    (1 << LABEL_16_BIT_)
#define LABEL_26_BIT    (1 << LABEL_26_BIT_)
#define LABEL_REFERENCE (1 << LABEL_RELATIVE_)
#define LABEL_DECLARE   (1 << LABEL_DECLARE_)

extern void symbol_table_init();
extern void symbol_table_fini();
extern void register_label(const char *label, int addr, int mode, int lineno);
extern void write_back_label_addr_all();

#endif // SYMBOL_H
