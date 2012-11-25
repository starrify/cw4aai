/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include "memu.h"
#include "decode.h"

/* 
 * reference: 
 *   MIPS® Architecture For Programmers Volume II-A: The MIPS32® Instruction Set
 */
  
/* Table A.2 MIPS32 Encoding of the Opcode Field */ 
static const u32_t opcode2inst[0100] = /* bit 31..26 */
{
    [000] = OP_SPECIAL | SYMBOL_LDELTA,
    [001] = OP_REGIMM | SYMBOL_LDELTA,
    [002] = INST_J,
    [003] = INST_JAL,
    [004] = INST_BEQ,
    [005] = INST_BNE,
    [006] = INST_BLEZ,
    [007] = INST_BGTZ,
    [010] = INST_ADDI,
    [011] = INST_ADDIU,
    [012] = INST_SLTI,
    [013] = INST_SLTIU,
    [014] = INST_ANDI,
    [015] = INST_ORI,
    [016] = INST_XORI,
    [017] = INST_LUI,
    [020] = OP_COP0 | SYMBOL_LDELTA,
    [021] = OP_COP1 | SYMBOL_LDELTA,
    [022] = OP_COP2 | SYMBOL_LDELTA | SYMBOL_LTHETA,
    [023] = OP_COP1X | SYMBOL_LDELTA,
    [024] = INST_BEQL | SYMBOL_LPHI,
    [025] = INST_BNEL | SYMBOL_LPHI,
    [026] = INST_BLEZL | SYMBOL_LPHI,
    [027] = INST_BGTZL | SYMBOL_LPHI,
    [030 ... 033] = INST_NONE | SYMBOL_LBETA,
    [034] = OP_SPECIAL2 | SYMBOL_LDELTA,
    [035] = INST_JALX | SYMBOL_LEPSILON,
    [036] = INST_NONE | SYMBOL_LEPSILON,
    [037] = OP_SPECIAL3 | SYMBOL_LDELTA | SYMBOL_OPLUS,
    [040] = INST_LB,
    [041] = INST_LH,
    [042] = INST_LWL,
    [043] = INST_LW,
    [044] = INST_LBU,
    [045] = INST_LHU,
    [046] = INST_LWR,
    [047] = INST_NONE | SYMBOL_LBETA,
    [050] = INST_SB,
    [051] = INST_SH,
    [052] = INST_SWL,
    [053] = INST_SW,
    [054] = INST_NONE | SYMBOL_LBETA,
    [055] = INST_NONE | SYMBOL_LBETA,
    [056] = INST_SWR,
    [057] = INST_CACHE,
    [060] = INST_LL,
    [061] = INST_LWC1,
    [062] = INST_LWC2 | SYMBOL_LTHETA,
    [063] = INST_PREF,
    [064] = INST_NONE | SYMBOL_LBETA,
    [065] = INST_LDC1,
    [066] = INST_LDC2 | SYMBOL_LTHETA,
    [067] = INST_NONE | SYMBOL_LBETA,
    [070] = INST_SC,
    [071] = INST_SWC1,
    [072] = INST_SWC2 | SYMBOL_LTHETA,
    [073] = INST_NONE | SYMBOL_ASTEROID,
    [074] = INST_NONE | SYMBOL_LBETA,
    [075] = INST_SDC1,
    [076] = INST_SDC2 | SYMBOL_LTHETA,
    [077] = INST_NONE | SYMBOL_LBETA,
};

/* Table A.3 MIPS32 SPECIAL Opcode Encoding of Function Field */
static const u32_t special2inst[0100] = /* bit 5..0 */
{
    [000] = INST_SLL,
    [001] = SPECIAL_MOVCI | SYMBOL_LDELTA,
    [002] = SPECIAL_SRL | SYMBOL_LDELTA,
    [003] = INST_SRA,
    [004] = INST_SLLV,
    [005] = INST_NONE | SYMBOL_ASTEROID,
    [006] = SPECIAL_SRLV | SYMBOL_LDELTA,
    [007] = INST_SRAV,
    [010] = INST_JR,
    [011] = INST_JALR,
    [012] = INST_MOVZ,
    [013] = INST_MOVN,
    [014] = INST_SYSCALL,
    [015] = INST_BREAK,
    [016] = INST_NONE | SYMBOL_ASTEROID,
    [017] = INST_SYNC,
    [020] = INST_MFHI,
    [021] = INST_MTHI,
    [022] = INST_MFLO,
    [023] = INST_MTLO,
    [024] = INST_NONE | SYMBOL_LBETA,
    [025] = INST_NONE | SYMBOL_ASTEROID,
    [026] = INST_NONE | SYMBOL_LBETA,
    [027] = INST_NONE | SYMBOL_LBETA,
    [030] = INST_MULT,
    [031] = INST_MULTU,
    [032] = INST_DIV,
    [033] = INST_DIVU,
    [034 ... 037] = INST_NONE | SYMBOL_LBETA,
    [040] = INST_ADD, 
    [041] = INST_ADDU,
    [042] = INST_SUB,
    [043] = INST_SUBU,
    [044] = INST_AND,
    [045] = INST_OR,
    [046] = INST_XOR,
    [047] = INST_NOR,
    [050] = INST_NONE | SYMBOL_ASTEROID,
    [051] = INST_NONE | SYMBOL_ASTEROID,
    [052] = INST_SLT,
    [053] = INST_SLTU,
    [054 ... 057] = INST_NONE | SYMBOL_LBETA,
    [060] = INST_TGE,
    [061] = INST_TGEU,
    [062] = INST_TLT,
    [063] = INST_TLTU,
    [064] = INST_TEQ,
    [065] = INST_NONE | SYMBOL_ASTEROID,
    [066] = INST_TNE,
    [067] = INST_NONE | SYMBOL_ASTEROID,
    [070] = INST_NONE | SYMBOL_LBETA,
    [071] = INST_NONE | SYMBOL_ASTEROID,
    [072 ... 074] = INST_NONE | SYMBOL_LBETA,
    [075] = INST_NONE | SYMBOL_ASTEROID,
    [076] = INST_NONE | SYMBOL_LBETA,
    [077] = INST_NONE | SYMBOL_LBETA,
};

/* Table A.4 MIPS32 REGIMM Encoding of rt Field */
static const u32_t regimm2inst[040] = /* bit 20..16 */
{
    [000] = INST_BLTZ,
    [001] = INST_BGEZ,
    [002] = INST_BLTZL | SYMBOL_LTHETA,
    [003] = INST_BGEZL | SYMBOL_LTHETA,
    [004 ... 007] = INST_NONE | SYMBOL_ASTEROID,
    [010] = INST_TGEI,
    [011] = INST_TGEIU,
    [012] = INST_TLTI,
    [013] = INST_TLTIU,
    [014] = INST_TEQI,
    [015] = INST_NONE | SYMBOL_ASTEROID,
    [016] = INST_TNEI,
    [017] = INST_NONE | SYMBOL_ASTEROID,
    [020] = INST_BLTZAL,
    [021] = INST_BGEZAL,
    [022] = INST_BLTZALL | SYMBOL_LTHETA,
    [023] = INST_BGEZALL | SYMBOL_LTHETA,
    [024 ... 036] = INST_NONE | SYMBOL_ASTEROID,
    [037] = INST_SYNCI | SYMBOL_OPLUS,
};

/* Table A.5 MIPS32 SPECIAL2 Encoding of Function Field */
static const u32_t special22inst[0100] = /* bit 5..0 */
{
    [000] = INST_MADD,
    [001] = INST_MADDU,
    [002] = INST_MUL,
    [003] = INST_NONE | SYMBOL_LTHETA,
    [004] = INST_MSUB,
    [005] = INST_MSUBU,
    [006 ... 037] = INST_NONE | SYMBOL_LTHETA,
    [040] = INST_CLZ,
    [041] = INST_CLO,
    [042] = INST_NONE | SYMBOL_LTHETA,
    [043] = INST_NONE | SYMBOL_LTHETA,
    [044] = INST_NONE | SYMBOL_LBETA,
    [045] = INST_NONE | SYMBOL_LBETA,
    [046 ... 076] = INST_NONE | SYMBOL_LTHETA,
    [077] = INST_SDBBP | SYMBOL_LDELTA,
};

/* Table A.6 MIPS32 SPECIAL3 Encoding of Function Field for Release 2 of the Architecture */
static const u32_t special32inst[0100] = /* bit 5..0 */
{
    [000] = INST_EXT | SYMBOL_OPLUS,
    [001 ... 003] = INST_NONE | SYMBOL_LBETA,
    [004] = INST_INS | SYMBOL_OPLUS,
    [005 ... 007] = INST_NONE | SYMBOL_LBETA,
    [010 ... 037] = INST_NONE | SYMBOL_ASTEROID,
    [040] = SPECIAL3_BSHFL | SYMBOL_OPLUS | SYMBOL_LDELTA,
    [041 ... 043] = INST_NONE | SYMBOL_ASTEROID,
    [044] = INST_NONE | SYMBOL_LBETA,
    [045 ... 072] = INST_NONE | SYMBOL_ASTEROID,
    [073] = INST_RDHWR | SYMBOL_OPLUS,
    [074 ... 077] = INST_NONE | SYMBOL_ASTEROID,
};

/* Table A.7 MIPS32 MOVCI Encoding of tf Bit */
static const u32_t movci2inst[0b10] = /* bit 16 */
{
    [0b0] = INST_MOVF,
    [0b1] = INST_MOVT,
};

/* Table A.8 MIPS32 SRL Encoding of Shift/Rotate */
static const u32_t srl2inst[0b10] = /* bit 21 */
{
    [0b0] = INST_SRL,
    [0b1] = INST_ROTR,
};

/* Table A.9 MIPS32 SRLV Encoding of Shift/Rotate */
static const u32_t srlv2inst[0b10] = /* bit 6 */
{
    [0b0] = INST_SRLV,
    [0b1] = INST_ROTRV,
};

/* Table A.10 MIPS32 BSHFL Encoding of sa Field */
static const u32_t bshfl2inst[040] = /* bit 10..6 */
{
    [000 ... 001] = INST_NONE,
    [002] = INST_WSBH,
    [003 ... 017] = INST_NONE,
    [020] = INST_SEB,
    [021 ... 027] = INST_NONE,
    [030] = INST_SEH,
    [031 ... 037] = INST_NONE,
};

/* Table A.11 MIPS32 COP0 Encoding of rs Field */
static const u32_t cop02inst[040] = /* bit 25..21 */
{
    [000] = INST_MFC0,
    [001] = INST_NONE | SYMBOL_LBETA,
    [002 ... 003] = INST_NONE | SYMBOL_ASTEROID,
    [004] = INST_MTC0,
    [005] = INST_NONE | SYMBOL_LBETA,
    [006 ... 011] = INST_NONE | SYMBOL_ASTEROID,
    [012] = INST_RDPGPR | SYMBOL_OPLUS,
    [013] = INST_MFMC0 | SYMBOL_LDELTA | SYMBOL_LOPLUS,
    [014 ... 015] = INST_NONE | SYMBOL_ASTEROID,
    [016] = INST_WRPGPR | SYMBOL_OPLUS,
    [017] = INST_NONE | SYMBOL_ASTEROID,
    [020 ... 037] = COP0_CO | SYMBOL_LDELTA,
};

/* Table A.12 MIPS32 COP0 Encoding of Function Field When rs=CO */
static const u32_t cop0_co2inst[0100] = /* bit 5..0 */
{
    [000] = INST_NONE | SYMBOL_ASTEROID,
    [001] = INST_TLBR,
    [002] = INST_TLBWI,
    [003 ... 005] = INST_NONE | SYMBOL_ASTEROID,
    [006] = INST_TLBWR,
    [007] = INST_NONE | SYMBOL_ASTEROID,
    [010] = INST_TLBP,
    [011 ... 027] = INST_NONE | SYMBOL_ASTEROID,
    [030] = INST_ERET,
    [031 ... 036] = INST_NONE | SYMBOL_ASTEROID,
    [037] = INST_DERET | SYMBOL_LSIGMA,
    [040] = INST_WAIT,
    [041 ... 077] = INST_NONE | SYMBOL_ASTEROID,
};

/* Table A.13 MIPS32 COP1 Encoding of rs Field */
static const u32_t cop12inst[040] = /* bit 25..21 */
{
    [000] = INST_MFC1, 
    [001] = INST_NONE | SYMBOL_LBETA,
    [002] = INST_CFC1,
    [003] = INST_MFHC1 | SYMBOL_OPLUS,
    [004] = INST_MTC1,
    [005] = INST_NONE | SYMBOL_LBETA,
    [006] = INST_CTC1,
    [007] = INST_MTHC1 | SYMBOL_OPLUS,
    [010] = INST_BC1 | SYMBOL_LDELTA,
    [011] = INST_BC1ANY2 | SYMBOL_LDELTA | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [012] = INST_BC1ANY4 | SYMBOL_LDELTA | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [013 ... 017] = INST_NONE | SYMBOL_ASTEROID,
    [020] = COP1_S | SYMBOL_LDELTA,
    [021] = COP1_D | SYMBOL_LDELTA,
    [022 ... 023] = INST_NONE | SYMBOL_ASTEROID,
    [024] = COP1_W | SYMBOL_LDELTA,
    [025] = COP1_L | SYMBOL_LDELTA,
    [026] = COP1_PS | SYMBOL_LDELTA,
    [027 ... 037] = INST_NONE | SYMBOL_ASTEROID,
};

/* Table A.14 MIPS32 COP1 Encoding of Function Field When rs=S */
static const u32_t cop1_s2inst[0100] = /* bit 5..0 */
{
    [000] = INST_ADD_FMT,
    [001] = INST_SUB_FMT,
    [002] = INST_MUL_FMT,
    [003] = INST_DIV_FMT,
    [004] = INST_SQRT_FMT,
    [005] = INST_ABS_FMT,
    [006] = INST_MOV_FMT,
    [007] = INST_NEG_FMT,
    [010] = INST_ROUND_L_FMT | SYMBOL_UDELTA,
    [011] = INST_TRUNC_L_FMT | SYMBOL_UDELTA,
    [012] = INST_CEIL_L_FMT | SYMBOL_UDELTA,
    [013] = INST_FLOOR_L_FMT | SYMBOL_UDELTA,
    [014] = INST_ROUND_W_FMT,
    [015] = INST_TRUNC_W_FMT,
    [016] = INST_CEIL_W_FMT,
    [017] = INST_FLOOR_W_FMT,
    [020] = INST_NONE | SYMBOL_ASTEROID,
    [021] = COP1_MOVCF | SYMBOL_LDELTA,
    [022] = INST_MOVZ_FMT,
    [023] = INST_MOVN_FMT,
    [024] = INST_NONE | SYMBOL_ASTEROID,
    [025] = INST_RECIP_FMT | SYMBOL_UDELTA,
    [026] = INST_RSQRT_FMT | SYMBOL_UDELTA,
    [027 ... 033] = INST_NONE | SYMBOL_ASTEROID,
    [034] = INST_RECIP2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [035] = INST_RECIP1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [036] = INST_RSQRT1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [037] = INST_RSQRT2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [040] = INST_NONE | SYMBOL_ASTEROID,
    [041] = INST_CVT_D_FMT,
    [042 ... 043] = INST_NONE | SYMBOL_ASTEROID,
    [044] = INST_CVT_W_FMT,
    [045] = INST_CVT_L_FMT | SYMBOL_UDELTA,
    [046] = INST_CVT_PS_S | SYMBOL_UDELTA,
    [047 ... 057] = INST_NONE | SYMBOL_ASTEROID,
    [060] = INST_C_F_FMT,
    [061] = INST_C_UN_FMT,
    [062] = INST_C_EQ_FMT,
    [063] = INST_C_UEQ_FMT,
    [064] = INST_C_OLT_FMT,
    [065] = INST_C_ULT_FMT,
    [066] = INST_C_OLE_FMT,
    [067] = INST_C_ULE_FMT,
    [070] = INST_C_SF_FMT,
    [071] = INST_C_NGLE_FMT,
    [072] = INST_C_SEQ_FMT,
    [073] = INST_C_NGL_FMT,
    [074] = INST_C_LT_FMT,
    [075] = INST_C_NGE_FMT,
    [076] = INST_C_LE_FMT,
    [077] = INST_C_NGT_FMT,
};

/* Table A.15 MIPS32 COP1 Encoding of Function Field When rs=D */
static const u32_t cop1_d2inst[0100] = /* bit 5..0 */
{
    [000] = INST_ADD_FMT,
    [001] = INST_SUB_FMT,
    [002] = INST_MUL_FMT,
    [003] = INST_DIV_FMT,
    [004] = INST_SQRT_FMT,
    [005] = INST_ABS_FMT,
    [006] = INST_MOV_FMT,
    [007] = INST_NEG_FMT,
    [010] = INST_ROUND_L_FMT | SYMBOL_UDELTA,
    [011] = INST_TRUNC_L_FMT | SYMBOL_UDELTA,
    [012] = INST_CEIL_L_FMT | SYMBOL_UDELTA,
    [013] = INST_FLOOR_L_FMT | SYMBOL_UDELTA,
    [014] = INST_ROUND_W_FMT,
    [015] = INST_TRUNC_W_FMT,
    [016] = INST_CEIL_W_FMT,
    [017] = INST_FLOOR_W_FMT,
    [020] = INST_NONE | SYMBOL_ASTEROID,
    [021] = COP1_MOVCF | SYMBOL_LDELTA,
    [022] = INST_MOVZ_FMT,
    [023] = INST_MOVN_FMT,
    [024] = INST_NONE | SYMBOL_ASTEROID,
    [025] = INST_RECIP_FMT | SYMBOL_UDELTA,
    [026] = INST_RSQRT_FMT | SYMBOL_UDELTA,
    [027 ... 033] = INST_NONE | SYMBOL_ASTEROID,
    [034] = INST_RECIP2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [035] = INST_RECIP1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [036] = INST_RSQRT1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [037] = INST_RSQRT2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [040] = INST_CVT_S_FMT,
    [041 ... 043] = INST_NONE | SYMBOL_ASTEROID,
    [044] = INST_CVT_W_FMT,
    [045] = INST_CVT_L_FMT | SYMBOL_UDELTA,
    [046 ... 057] = INST_NONE | SYMBOL_ASTEROID,
    [060] = INST_C_F_FMT,
    [061] = INST_C_UN_FMT,
    [062] = INST_C_EQ_FMT,
    [063] = INST_C_UEQ_FMT,
    [064] = INST_C_OLT_FMT,
    [065] = INST_C_ULT_FMT,
    [066] = INST_C_OLE_FMT,
    [067] = INST_C_ULE_FMT,
    [070] = INST_C_SF_FMT,
    [071] = INST_C_NGLE_FMT,
    [072] = INST_C_SEQ_FMT,
    [073] = INST_C_NGL_FMT,
    [074] = INST_C_LT_FMT,
    [075] = INST_C_NGE_FMT,
    [076] = INST_C_LE_FMT,
    [077] = INST_C_NGT_FMT,
};

/* Table A.16 MIPS32 COP1 Encoding of Function Field When rs=W or L */
static const u32_t cop1_w_l2inst[0100] = /* bit 5..0 */
{
    [000 ... 037] = INST_NONE | SYMBOL_ASTEROID,
    [040] = INST_CVT_S_FMT,
    [041] = INST_CVT_D_FMT,
    [042 ... 045] = INST_NONE | SYMBOL_ASTEROID,
    [046] = INST_CVT_PS_PW | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [047 ... 077] = INST_NONE | SYMBOL_ASTEROID,
};

/* Table A.17 MIPS32 COP1 Encoding of Function Field When rs=PS */
static const u32_t cop1_ps2inst[0100] = /* bit 5..0 */
{
    [000] = INST_ADD_FMT | SYMBOL_UDELTA,
    [001] = INST_SUB_FMT | SYMBOL_UDELTA,
    [002] = INST_MUL_FMT | SYMBOL_UDELTA,
    [003] = INST_NONE | SYMBOL_ASTEROID,
    [004] = INST_NONE | SYMBOL_ASTEROID,
    [005] = INST_ABS_FMT | SYMBOL_UDELTA,
    [006] = INST_MOV_FMT | SYMBOL_UDELTA,
    [007] = INST_NEG_FMT | SYMBOL_UDELTA,
    [010 ... 020] = INST_NONE | SYMBOL_ASTEROID,
    [021] = COP1_MOVCF | SYMBOL_LDELTA | SYMBOL_UDELTA,
    [022] = INST_MOVZ_FMT | SYMBOL_UDELTA,
    [023] = INST_MOVN_FMT | SYMBOL_UDELTA,
    [024 ... 027] = INST_NONE | SYMBOL_ASTEROID,
    [030] = INST_ADDR_PS | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [031] = INST_NONE | SYMBOL_ASTEROID,
    [032] = INST_MULR_PS | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [033] = INST_NONE | SYMBOL_ASTEROID,
    [034] = INST_RECIP2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [035] = INST_RECIP1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [036] = INST_RSQRT1 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [037] = INST_RSQRT2 | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [040] = INST_CVT_S_PU | SYMBOL_UDELTA,
    [041 ... 043] = INST_NONE | SYMBOL_ASTEROID,
    [044] = INST_CVT_PW_PS | SYMBOL_LEPSILON | SYMBOL_UDELTA,
    [045 ... 047] = INST_NONE | SYMBOL_ASTEROID,
    [050] = INST_CVT_S_PL | SYMBOL_UDELTA,
    [051 ... 053] = INST_NONE | SYMBOL_ASTEROID,
    [054] = INST_PLL_PS | SYMBOL_UDELTA,
    [055] = INST_PLU_PS | SYMBOL_UDELTA,
    [056] = INST_PUL_PS | SYMBOL_UDELTA,
    [057] = INST_PUU_PS | SYMBOL_UDELTA,
    [060] = INST_C_F_FMT | SYMBOL_UDELTA,
    [061] = INST_C_UN_FMT | SYMBOL_UDELTA,
    [062] = INST_C_EQ_FMT | SYMBOL_UDELTA,
    [063] = INST_C_UEQ_FMT | SYMBOL_UDELTA,
    [064] = INST_C_OLT_FMT | SYMBOL_UDELTA,
    [065] = INST_C_ULT_FMT | SYMBOL_UDELTA,
    [066] = INST_C_OLE_FMT | SYMBOL_UDELTA,
    [067] = INST_C_ULE_FMT | SYMBOL_UDELTA,
    [070] = INST_C_SF_FMT | SYMBOL_UDELTA,
    [071] = INST_C_NGLE_FMT | SYMBOL_UDELTA,
    [072] = INST_C_SEQ_FMT | SYMBOL_UDELTA,
    [073] = INST_C_NGL_FMT | SYMBOL_UDELTA,
    [074] = INST_C_LT_FMT | SYMBOL_UDELTA,
    [075] = INST_C_NGE_FMT | SYMBOL_UDELTA,
    [076] = INST_C_LE_FMT | SYMBOL_UDELTA,
    [077] = INST_C_NGT_FMT | SYMBOL_UDELTA,
};

/* Table A.18 MIPS32 COP1 Encoding of tf Bit When rs=S, D, or PS, Function=MOVCF */
static const u32_t cop1_movcf2inst[0b10] = /* bit 16 */
{
    [0b0] = INST_MOVF_FMT,
    [0b1] = INST_MOVT_FMT,
};

/* Table A.19 MIPS32 COP2 Encoding of rs Field */
static const u32_t cop22inst[040] = /* bit 25..21 */
{
    [000] = INST_MFC2 | SYMBOL_LTHETA,
    [001] = INST_NONE | SYMBOL_LBETA,
    [002] = INST_CFC2 | SYMBOL_LTHETA,
    [003] = INST_MFHC2 | SYMBOL_LTHETA | SYMBOL_OPLUS,
    [004] = INST_MTC2 | SYMBOL_LTHETA,
    [005] = INST_NONE | SYMBOL_LBETA,
    [006] = INST_CTC2 | SYMBOL_LTHETA,
    [007] = INST_MTHC2 | SYMBOL_LTHETA | SYMBOL_OPLUS,
    [010] = INST_BC2 | SYMBOL_LTHETA,
    [011 ... 017] = INST_NONE | SYMBOL_ASTEROID,
    [020 ... 037] = COP2_C2 | SYMBOL_LTHETA | SYMBOL_ASTEROID,
};

/* Table A.20 MIPS64 COP1X Encoding of Function Field */
static const u32_t cop1x2inst[0100] = /* bit 5..0 */
{
    [000] = INST_LWXC1 | SYMBOL_UDELTA,
    [001] = INST_LDXC1 | SYMBOL_UDELTA,
    [002 ... 004] = INST_NONE | SYMBOL_ASTEROID,
    [005] = INST_LUXC1 | SYMBOL_UDELTA,
    [006 ... 7] = INST_NONE | SYMBOL_ASTEROID,
    [010] = INST_SWXC1 | SYMBOL_UDELTA,
    [011] = INST_SDXC1 | SYMBOL_UDELTA,
    [012 ... 014] = INST_NONE | SYMBOL_ASTEROID,
    [015] = INST_SUXC1 | SYMBOL_UDELTA,
    [016] = INST_NONE | SYMBOL_ASTEROID,
    [017] = INST_PREFX | SYMBOL_UDELTA,
    [020 ... 035] = INST_NONE | SYMBOL_ASTEROID,
    [036] = INST_ALNV_PS | SYMBOL_UDELTA,
    [037] = INST_NONE | SYMBOL_ASTEROID,
    [040] = INST_MADD_S | SYMBOL_UDELTA,
    [041] = INST_MADD_D | SYMBOL_UDELTA,
    [042 ... 45] = INST_NONE | SYMBOL_ASTEROID,
    [046] = INST_MADD_PS | SYMBOL_UDELTA,
    [047] = INST_NONE | SYMBOL_ASTEROID,
    [050] = INST_MSUB_S | SYMBOL_UDELTA,
    [051] = INST_MSUB_D | SYMBOL_UDELTA,
    [052 ... 055] = INST_NONE | SYMBOL_ASTEROID,
    [056] = INST_MSUB_PS | SYMBOL_UDELTA,
    [057] = INST_NONE | SYMBOL_ASTEROID,
    [060] = INST_NMADD_S | SYMBOL_UDELTA,
    [061] = INST_NMADD_D | SYMBOL_UDELTA,
    [062 ... 065] = INST_NONE | SYMBOL_ASTEROID,
    [066] = INST_NMADD_PS | SYMBOL_UDELTA,
    [067] = INST_NONE | SYMBOL_ASTEROID,
    [070] = INST_NMSUB_S | SYMBOL_UDELTA,
    [071] = INST_NMSUB_D | SYMBOL_UDELTA,
    [072 ... 075] = INST_NONE | SYMBOL_ASTEROID,
    [076] = INST_NMSUB_PS | SYMBOL_UDELTA,
    [077] = INST_NONE | SYMBOL_ASTEROID,
};

static inline u32_t decode_cop1_movcf(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1_movcf2inst[MASKSHR(bincode, 16, 16)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_special_movci(u32_t bincode)
{
    u32_t ret;
    u32_t inst = movci2inst[MASKSHR(bincode, 16, 16)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_special_srl(u32_t bincode)
{
    u32_t ret;
    u32_t inst = srl2inst[MASKSHR(bincode, 21, 21)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_special_srlv(u32_t bincode)
{
    u32_t ret;
    u32_t inst = srlv2inst[MASKSHR(bincode, 6, 6)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_special3_bshfl(u32_t bincode)
{
    u32_t ret;
    u32_t inst = bshfl2inst[MASKSHR(bincode, 6, 6)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_cop0_co(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop0_co2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_cop1_s(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1_s2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    case COP1_MOVCF:
        ret = inst & MASK_31_16 | decode_cop1_movcf(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_cop1_d(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1_d2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    case COP1_MOVCF:
        ret = inst & MASK_31_16 | decode_cop1_movcf(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_cop1_w_l(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1_w_l2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_cop1_ps(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1_ps2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    case COP1_MOVCF:
        ret = inst & MASK_31_16 | decode_cop1_movcf(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_special(u32_t bincode)
{
    u32_t ret;
    u32_t inst = special2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    case SPECIAL_MOVCI:
        ret = inst & MASK_31_16 | decode_special_movci(bincode);
        break;
    case SPECIAL_SRL:
        ret = inst & MASK_31_16 | decode_special_srl(bincode);
        break;
    case SPECIAL_SRLV:
        ret = inst & MASK_31_16 | decode_special_srlv(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_regimm(u32_t bincode)
{
    u32_t ret;
    u32_t inst = regimm2inst[MASKSHR(bincode, 20, 16)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_special2(u32_t bincode)
{
    u32_t ret;
    u32_t inst = special22inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_special3(u32_t bincode)
{
    u32_t ret;
    u32_t inst = special32inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    case SPECIAL3_BSHFL:
        ret = inst & MASK_31_16 | decode_special3_bshfl(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_cop0(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop02inst[MASKSHR(bincode, 25, 21)];
    switch (inst & MASK_15_0)
    {
    case COP0_CO:
        ret = inst & MASK_31_16 | decode_cop0_co(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_cop1(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop12inst[MASKSHR(bincode, 25, 21)];
    switch (inst & MASK_15_0)
    {
    case COP1_S:
        ret = inst & MASK_31_16 | decode_cop1_s(bincode);
        break;
    case COP1_D:
        ret = inst & MASK_31_16 | decode_cop1_d(bincode);
        break;
    case COP1_W:
    case COP1_L:
        ret = inst & MASK_31_16 | decode_cop1_w_l(bincode);
        break;
    case COP1_PS:
        ret = inst & MASK_31_16 | decode_cop1_ps(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_cop2(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop22inst[MASKSHR(bincode, 25, 21)];
    switch (inst & MASK_15_0)
    {
    case COP2_C2:
        ret = inst & MASK_31_16 | INST_NONE; // not specified in the manual
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_op_cop1x(u32_t bincode)
{
    u32_t ret;
    u32_t inst = cop1x2inst[MASKSHR(bincode, 5, 0)];
    switch (inst & MASK_15_0)
    {
    default:
        ret = inst;
        break;
    }
    return ret;
}

static inline u32_t decode_opcode(u32_t bincode)
{
    u32_t ret;
    u32_t inst = opcode2inst[MASKSHR(bincode, 31, 26)];
    switch (inst & MASK_15_0)
    {
    case OP_SPECIAL:
        ret = inst & MASK_31_16 | decode_op_special(bincode);
        break;
    case OP_REGIMM:
        ret = inst & MASK_31_16 | decode_op_regimm(bincode);
        break;      
    case OP_COP0:
        ret = inst & MASK_31_16 | decode_op_cop0(bincode);
        break;
    case OP_COP1:
        ret = inst & MASK_31_16 | decode_op_cop1(bincode);
        break;
    case OP_COP2:
        ret = inst & MASK_31_16 | decode_op_cop2(bincode);
        break;
    case OP_COP1X:
        ret = inst & MASK_31_16 | decode_op_cop1x(bincode);
        break;
    case OP_SPECIAL2:
        ret = inst & MASK_31_16 | decode_op_special2(bincode);
        break;
    case OP_SPECIAL3:
        ret = inst & MASK_31_16 | decode_op_special3(bincode);
        break;
    default:
        ret = inst;
        break;
    }
    return ret;
}

u32_t decode(u32_t code)
{
    u32_t ret = decode_opcode(code);
//    ret &= ~SYMBOL_LDELTA; /* all other symbols would cause a Reserved Instruction Exception */ 
    ret &= 0xFFFF; // clear symbols
    return ret;
}

