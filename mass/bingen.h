/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef BINGEN_H
#define BINGEN_H

#define INST_NULL   INST_ADD 
#define REG_NULL REG_ZERO

#define MASK_LOW5   0x1f
#define MASK_LOW6   0x3f
#define MASK_LOW16  0xffff
#define MASK_LOW26  0x3ffffff

extern unsigned int buf_offset;

extern void buf_init();
extern unsigned int buf_load_word(unsigned int pcount);
extern void buf_save_word(unsigned int pcount, unsigned int code);
extern void buf_dump_to_file(FILE *fout);
extern void buf_fini();
extern unsigned int getpcount();
extern int getlineno();
extern void write_code(unsigned int code);

extern void r_type_inst_assemble(int inst, int rs, int rt, int rd, int sa);
extern void i_type_inst_assemble_immediate(int inst, int rs, int rt, int immediate);
extern void i_type_inst_assemble_label(int inst, int rs, int rt, const char *label);
extern void j_type_inst_assemble(int inst, const char *label);
extern void cp1_inst_assemble(int inst, int ft, int fs, int fd);

#endif // BINGEN_H
