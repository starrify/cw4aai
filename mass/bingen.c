/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mass.h"
#include "mass.tab.h"
#include "mass.yy.h"
#include "bingen.h"
#include "symbol.h"

#define SWAP_SHORT(a, b)    ({ short tmp = a; a = b; b = tmp; })
#define SWAP_CHAR(a, b) ({ char tmp = a; a = b; b = tmp; })

#define INIT_BUFSIZE    0x100

/* for those wants to be assembed from a base address other than 0.. */
unsigned int buf_offset = 0;

static unsigned int reg[] = 
{
    [REG_ZERO] = 0x0,
    [REG_AT] = 0x1,
    [REG_V0] = 0x2,
    [REG_V1] = 0x3,
    [REG_A0] = 0x4,
    [REG_A1] = 0x5,
    [REG_A2] = 0x6,
    [REG_A3] = 0x7,
    [REG_T0] = 0x8,
    [REG_T1] = 0x9,
    [REG_T2] = 0xa,
    [REG_T3] = 0xb,
    [REG_T4] = 0xc,
    [REG_T5] = 0xd,
    [REG_T6] = 0xe,
    [REG_T7] = 0xf,
    [REG_S0] = 0x10,
    [REG_S1] = 0x11,
    [REG_S2] = 0x12,
    [REG_S3] = 0x13,
    [REG_S4] = 0x14,
    [REG_S5] = 0x15,
    [REG_S6] = 0x16,
    [REG_S7] = 0x17,
    [REG_T8] = 0x18,
    [REG_T9] = 0x19,
    [REG_K0] = 0x1a,
    [REG_K1] = 0x1b,
    [REG_GP] = 0x1c,
    [REG_SP] = 0x1d,
    [REG_FP] = 0x1e,
    [REG_RA] = 0x1f
};

static unsigned int opcode[] = 
{
    /* all opcodes for r-type instructions are 0b000000 */

    /* i-type instructions */
    [INST_ADDI] = 0b001000,
    [INST_ADDIU]    = 0b001001,
    [INST_ANDI] = 0b001100,
    [INST_BEQ]  = 0b000100,
    [INST_BGEZ] = 0b000001,
    [INST_BGTZ] = 0b000111,
    [INST_BLEZ] = 0b000110,
    [INST_BLTZ] = 0b000001,
    [INST_BNE]  = 0b000101,
    [INST_LB]   = 0b100000,
    [INST_LBU]  = 0b100100,
    [INST_LH]   = 0b100001,
    [INST_LHU]  = 0b100101,
    [INST_LUI]  = 0b001111,
    [INST_LW]   = 0b100011,
    [INST_LWC1] = 0b110001,
    [INST_ORI]  = 0b001101,
    [INST_SB]   = 0b101000,
    [INST_SLTI] = 0b001010,
    [INST_SLTIU]    = 0b001011,
    [INST_SH]   = 0b101001,
    [INST_SW]   = 0b101011,
    [INST_SWC1] = 0b111001,
    [INST_XORI] = 0b001110,

    /* j-type instructions */
    [INST_J]    = 0b000010,
    [INST_JAL]  = 0b000011

    /* all opcodes for cp0 instructions are 0b010000 */
    /* all opcodes for cp1 instructions are 0b010001 */
};

static unsigned int function[] = 
{
    /* r-type instructions */
    [INST_ADD]  = 0b100000,
    [INST_ADDU] = 0b100001,
    [INST_AND]  = 0b100100,
    [INST_BREAK]    = 0b001101,
    [INST_DIV]  = 0b011010,
    [INST_DIVU] = 0b011011,
    [INST_JALR] = 0b001001,
    [INST_JR]   = 0b001000,
    [INST_MFHI] = 0b010000,
    [INST_MFLO] = 0b010010,
    [INST_MTHI] = 0b010001,
    [INST_MTLO] = 0b010011,
    [INST_MULT] = 0b011000,
    [INST_MULTU]    = 0b011001,
    [INST_NOR]  = 0b100111,
    [INST_OR]   = 0b100101,
    [INST_SLL]  = 0b000000,
    [INST_SLLV] = 0b000100,
    [INST_SLT]  = 0b101010,
    [INST_SLTU] = 0b101011,
    [INST_SRA]  = 0b000011,
    [INST_SRAV] = 0b000111,
    [INST_SRL]  = 0b000010,
    [INST_SRLV] = 0b000110,
    [INST_SUB]  = 0b100010,
    [INST_SUBU] = 0b100011,
    [INST_SYSCALL]  = 0b001100,
    [INST_XOR]  = 0b100110,

    /* no function for i/j-type instructions */

    /* cp0 instructions*/
    [INST_MFC0] = 0b000000,
    [INST_MTC0] = 0b000000,
    [INST_ERET] = 0b011000,
    [INST_WAIT] = 0b100000,
    [INST_RDPGPR] = 0b000000,
    [INST_WRPGPR] = 0b000000,

    /* cp1 instructions */
    [INST_ADD_S]    = 0b000000,
    [INST_CVT_S_W]  = 0b100000,
    [INST_CVT_W_S]  = 0b100100,
    [INST_DIV_S]    = 0b000011,
    [INST_MFC1] = 0b000000,
    [INST_MOV_S]    = 0b000110,
    [INST_MTC1] = 0b000000,
    [INST_MUL_S]    = 0b000010,
    [INST_SUB_S]    = 0b000001,

};

static unsigned int format[] =
{
    /* for c0 instructions */
    [INST_ERET] = 0b10000,
    [INST_WAIT] = 0b10000,
    [INST_MFC0] = 0b00000,
    [INST_MTC0] = 0b00100,
    [INST_RDPGPR]   = 0b01010,
    [INST_WRPGPR]   = 0b01110,

    /* for c1 instructions */
    [INST_ADD_S]    = 0b10000,
    [INST_CVT_S_W]  = 0b10100,
    [INST_CVT_W_S]  = 0b10000,
    [INST_DIV_S]    = 0b10000,
    [INST_MFC1] = 0b00000,
    [INST_MOV_S]    =0b10000,
    [INST_MTC1] = 0b00100,
    [INST_MUL_S]    = 0b10000,
    [INST_SUB_S]    = 0b10000,
};

static unsigned int *buf;
static unsigned int bufsize;
static unsigned int pcount;

void buf_init()
{
    assert(!buf);
    bufsize = INIT_BUFSIZE;
    pcount = 0; 
    buf = malloc(bufsize * sizeof(*buf));
    assert(buf);
    return;
}

static void buf_resize(unsigned int newsize)
{
    bufsize = newsize;
    buf = realloc(buf, bufsize * sizeof(*buf));
    assert(buf);
    return;
}

unsigned int buf_load_word(unsigned int pcount)
{
    return buf[pcount >> 2];
}

void buf_save_word(unsigned int pcount, unsigned int code)
{
    buf[pcount >> 2] = code;
    return;
}

void buf_dump_to_file(FILE *fout)
{
    if (option_byteorder == BYTEORDER_BE)
    {
        int i;
        for (i = 0; i < pcount >> 2; i++)
        {
            SWAP_SHORT(*(short*)&buf[i], *(((short*)&buf[i]) + 1));
            SWAP_CHAR(*(char*)&buf[i], *(((char*)&buf[i]) + 1));
            SWAP_CHAR(*(((char*)&buf[i]) + 2), *(((char*)&buf[i]) + 3));
        }
    }
    else
        ; /* assuming this program runs on a little endian machine */
    fwrite((void*)buf + buf_offset, pcount - buf_offset, 1, fout);
    
    if (option_output_binary)
    {
        /* debug code only... */
        int i;
        for (i = 0; i < pcount >> 2; i++)
        {
            int j;
            int tmp_code = buf[i];
            for (j = 0; j < 32; j++)
            {
                fprintf(stdout, "%d", tmp_code & 0x80000000 ? 1 : 0);
                tmp_code <<= 1;
            }
            fprintf(stdout, "\n");
        }
    }
    return;
}

void buf_fini()
{
    assert(buf);
    pcount = 0;
    bufsize = 0;
    free(buf);
    return;
}

/* reverse the byte order of a 4-byte word */
static inline byteorder_reverse(unsigned int code)
{
    SWAP_SHORT(*(short*)&code, *(((short*)&code) + 1));
    SWAP_CHAR(*(char*)&code, *(((char*)&code) + 1));
    SWAP_CHAR(*(((char*)&code) + 2), *(((char*)&code) + 3));
    return code;
}

/* write binary code to buffer */
static void inline write_to_buf(unsigned int code)
{
    if (pcount >= bufsize)
        buf_resize(bufsize * 2);
        
    buf[pcount >> 2] = code;
    pcount += 4;

    return;
}

void write_code(unsigned int code)
{
    write_to_buf(code);
    return;
}

/* get current program count */
unsigned int getpcount()
{
    return pcount;
}

void r_type_inst_assemble(int inst, int rs, int rt, int rd, int sa)
{   
    /*
     * binary layout of r-type instructions:
     *  opcode(6) rs(5) rt(5) rd(5) sa(5) function(6)
     */
    unsigned int code = 0;
    code |= 0b000000 << 26; /* opcode */
    code |= (reg[rs] & MASK_LOW5) << 21; /* rs */
    code |= (reg[rt] & MASK_LOW5) << 16; /* rt */
    code |= (reg[rd] & MASK_LOW5) << 11; /* rd */
    code |= (sa & MASK_LOW5) << 6; /* sa */ 
    code |= (function[inst] & MASK_LOW6) << 0; /* function */
    write_to_buf(code);
    return;    
}

void i_type_inst_assemble_immediate(int inst, int rs, int rt, int immediate)
{
    /*
     * binary layout of i-type instructions:
     *  opcode(6) rs(5) rt(5) immediate(16)
     */
    unsigned int code = 0;
    code |= (opcode[inst] & MASK_LOW6) << 26; /* opcode */
    code |= (reg[rs] & MASK_LOW5) << 21; /* rs */
    code |= (reg[rt] & MASK_LOW5) << 16; /* rt*/
    code |= (immediate & MASK_LOW16) << 0; /* immediate */
    write_to_buf(code);
    return;
}

void i_type_inst_assemble_label(int inst, int rs, int rt, const char *label)
{
    /*
     * binary layout of r-type instructions:
     *  opcode(6) rs(5) rt(5) immediate(16)
     */
    unsigned int pc = getpcount();
    unsigned int lineno = yyget_lineno();
    register_label(label, pc, LABEL_RELATIVE | LABEL_16_BIT | LABEL_REFERENCE, lineno);
    i_type_inst_assemble_immediate(inst, rs, rt, 0);
    return;
}

void j_type_inst_assemble(int inst, const char *label)
{
    /*
     * binary layout of j-type instructions:
     * opcode(6) target(26)
     */
    unsigned int pc = getpcount();
    unsigned int mode = LABEL_26_BIT | LABEL_REFERENCE | LABEL_ABSOLUTE;
    unsigned int lineno = yyget_lineno();
    register_label(label, pc, mode, lineno);
    unsigned int code = 0;
    code |= (opcode[inst] & MASK_LOW6) << 26; /* opcode */
    code |= (0 & MASK_LOW26) << 0; /* target */
    write_to_buf(code);

    return;
}

void cp0_inst_assemble(int inst, int rt, int rd)
{
    /*
     * binary layout of cp0_inst_assemble:
     *  opcode(6) rs(5) rt(5) rd(5) misc(5) function(6)
     */
    unsigned int code = 0;
    code |= (0b010000 & MASK_LOW6) << 26; /* opcode */
    code |= (format[inst] & MASK_LOW5) << 21; /* format */
    code |= (reg[rt] & MASK_LOW5) << 16; /* rt */
    code |= (reg[rd] & MASK_LOW5) << 11; /* rd */
    code |= (0b000000 & MASK_LOW6) << 5; /* misc */
    code |= (function[inst] & MASK_LOW6) << 0; /* function */
    write_to_buf(code);
    return;
}

void cp1_inst_assemble(int inst, int ft, int fs, int fd)
{
    /*
     * binary layout of cp1_inst_assemble:
     *  opcode(6) format(5) ft(5) fs(5) fd(5) function(6)
     */
    unsigned int code = 0;
    code |= (0b010001 & MASK_LOW6) << 26; /* opcode */
    code |= (format[inst] & MASK_LOW5) << 21; /* format */
    code |= (reg[ft] & MASK_LOW5) << 16; /* ft */
    code |= (reg[fs] & MASK_LOW5) << 11; /* fs */
    code |= (reg[fd] & MASK_LOW5) << 6; /* fd */
    code |= (function[inst] & MASK_LOW6) << 0; /* function */
    write_to_buf(code);
    return;
}

