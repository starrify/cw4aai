/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "exception.h"
#include "mem.h"
#include "mmu.h"
#include "memu.h"
#include "decode.h"
#include "reg.h"

static int inst_not_implemented(u32_t code);
static int inst_exec_abs_fmt(u32_t code);
static int inst_exec_add(u32_t code);
static int inst_exec_addi(u32_t code);
static int inst_exec_addiu(u32_t code);
static int inst_exec_addr_ps(u32_t code);
static int inst_exec_addu(u32_t code);
static int inst_exec_add_fmt(u32_t code);
static int inst_exec_alnv_ps(u32_t code);
static int inst_exec_and(u32_t code);
static int inst_exec_andi(u32_t code);
static int inst_exec_bc1(u32_t code);
static int inst_exec_bc1any2(u32_t code);
static int inst_exec_bc1any4(u32_t code);
static int inst_exec_bc2(u32_t code);
static int inst_exec_beq(u32_t code);
static int inst_exec_beql(u32_t code);
static int inst_exec_bgez(u32_t code);
static int inst_exec_bgezal(u32_t code);
static int inst_exec_bgezall(u32_t code);
static int inst_exec_bgezl(u32_t code);
static int inst_exec_bgtz(u32_t code);
static int inst_exec_bgtzl(u32_t code);
static int inst_exec_blez(u32_t code);
static int inst_exec_blezl(u32_t code);
static int inst_exec_bltz(u32_t code);
static int inst_exec_bltzal(u32_t code);
static int inst_exec_bltzall(u32_t code);
static int inst_exec_bltzl(u32_t code);
static int inst_exec_bne(u32_t code);
static int inst_exec_bnel(u32_t code);
static int inst_exec_break(u32_t code);
static int inst_exec_cache(u32_t code);
static int inst_exec_ceil_l_fmt(u32_t code);
static int inst_exec_ceil_w_fmt(u32_t code);
static int inst_exec_cfc1(u32_t code);
static int inst_exec_cfc2(u32_t code);
static int inst_exec_clo(u32_t code);
static int inst_exec_clz(u32_t code);
static int inst_exec_ctc1(u32_t code);
static int inst_exec_ctc2(u32_t code);
static int inst_exec_cvt_d_fmt(u32_t code);
static int inst_exec_cvt_l_fmt(u32_t code);
static int inst_exec_cvt_ps_pw(u32_t code);
static int inst_exec_cvt_ps_s(u32_t code);
static int inst_exec_cvt_pw_ps(u32_t code);
static int inst_exec_cvt_s_fmt(u32_t code);
static int inst_exec_cvt_s_pl(u32_t code);
static int inst_exec_cvt_s_pu(u32_t code);
static int inst_exec_cvt_w_fmt(u32_t code);
static int inst_exec_c_eq_fmt(u32_t code);
static int inst_exec_c_f_fmt(u32_t code);
static int inst_exec_c_le_fmt(u32_t code);
static int inst_exec_c_lt_fmt(u32_t code);
static int inst_exec_c_nge_fmt(u32_t code);
static int inst_exec_c_ngl_fmt(u32_t code);
static int inst_exec_c_ngle_fmt(u32_t code);
static int inst_exec_c_ngt_fmt(u32_t code);
static int inst_exec_c_ole_fmt(u32_t code);
static int inst_exec_c_olt_fmt(u32_t code);
static int inst_exec_c_seq_fmt(u32_t code);
static int inst_exec_c_sf_fmt(u32_t code);
static int inst_exec_c_ueq_fmt(u32_t code);
static int inst_exec_c_ule_fmt(u32_t code);
static int inst_exec_c_ult_fmt(u32_t code);
static int inst_exec_c_un_fmt(u32_t code);
static int inst_exec_deret(u32_t code);
static int inst_exec_div(u32_t code);
static int inst_exec_divu(u32_t code);
static int inst_exec_div_fmt(u32_t code);
static int inst_exec_eret(u32_t code);
static int inst_exec_ext(u32_t code);
static int inst_exec_floor_l_fmt(u32_t code);
static int inst_exec_floor_w_fmt(u32_t code);
static int inst_exec_ins(u32_t code);
static int inst_exec_j(u32_t code);
static int inst_exec_jal(u32_t code);
static int inst_exec_jalr(u32_t code);
static int inst_exec_jalx(u32_t code);
static int inst_exec_jr(u32_t code);
static int inst_exec_lb(u32_t code);
static int inst_exec_lbu(u32_t code);
static int inst_exec_ldc1(u32_t code);
static int inst_exec_ldc2(u32_t code);
static int inst_exec_ldxc1(u32_t code);
static int inst_exec_lh(u32_t code);
static int inst_exec_lhu(u32_t code);
static int inst_exec_ll(u32_t code);
static int inst_exec_lui(u32_t code);
static int inst_exec_luxc1(u32_t code);
static int inst_exec_lw(u32_t code);
static int inst_exec_lwc1(u32_t code);
static int inst_exec_lwc2(u32_t code);
static int inst_exec_lwl(u32_t code);
static int inst_exec_lwr(u32_t code);
static int inst_exec_lwxc1(u32_t code);
static int inst_exec_madd(u32_t code);
static int inst_exec_maddu(u32_t code);
static int inst_exec_madd_d(u32_t code);
static int inst_exec_madd_ps(u32_t code);
static int inst_exec_madd_s(u32_t code);
static int inst_exec_mfc0(u32_t code);
static int inst_exec_mfc1(u32_t code);
static int inst_exec_mfc2(u32_t code);
static int inst_exec_mfhc1(u32_t code);
static int inst_exec_mfhc2(u32_t code);
static int inst_exec_mfhi(u32_t code);
static int inst_exec_mflo(u32_t code);
static int inst_exec_mfmc0(u32_t code);
static int inst_exec_movf(u32_t code);
static int inst_exec_movf_fmt(u32_t code);
static int inst_exec_movn(u32_t code);
static int inst_exec_movn_fmt(u32_t code);
static int inst_exec_movt(u32_t code);
static int inst_exec_movt_fmt(u32_t code);
static int inst_exec_movz(u32_t code);
static int inst_exec_movz_fmt(u32_t code);
static int inst_exec_mov_fmt(u32_t code);
static int inst_exec_msub(u32_t code);
static int inst_exec_msubu(u32_t code);
static int inst_exec_msub_d(u32_t code);
static int inst_exec_msub_ps(u32_t code);
static int inst_exec_msub_s(u32_t code);
static int inst_exec_mtc0(u32_t code);
static int inst_exec_mtc1(u32_t code);
static int inst_exec_mtc2(u32_t code);
static int inst_exec_mthc1(u32_t code);
static int inst_exec_mthc2(u32_t code);
static int inst_exec_mthi(u32_t code);
static int inst_exec_mtlo(u32_t code);
static int inst_exec_mul(u32_t code);
static int inst_exec_mulr_ps(u32_t code);
static int inst_exec_mult(u32_t code);
static int inst_exec_multu(u32_t code);
static int inst_exec_mul_fmt(u32_t code);
static int inst_exec_neg_fmt(u32_t code);
static int inst_exec_nmadd_d(u32_t code);
static int inst_exec_nmadd_ps(u32_t code);
static int inst_exec_nmadd_s(u32_t code);
static int inst_exec_nmsub_d(u32_t code);
static int inst_exec_nmsub_ps(u32_t code);
static int inst_exec_nmsub_s(u32_t code);
static int inst_exec_nor(u32_t code);
static int inst_exec_or(u32_t code);
static int inst_exec_ori(u32_t code);
static int inst_exec_pll_ps(u32_t code);
static int inst_exec_plu_ps(u32_t code);
static int inst_exec_pref(u32_t code);
static int inst_exec_prefx(u32_t code);
static int inst_exec_pul_ps(u32_t code);
static int inst_exec_puu_ps(u32_t code);
static int inst_exec_rdhwr(u32_t code);
static int inst_exec_rdpgpr(u32_t code);
static int inst_exec_recip_fmt(u32_t code);
static int inst_exec_recip1(u32_t code);
static int inst_exec_recip2(u32_t code);
static int inst_exec_rotr(u32_t code);
static int inst_exec_rotrv(u32_t code);
static int inst_exec_round_l_fmt(u32_t code);
static int inst_exec_round_w_fmt(u32_t code);
static int inst_exec_rsqrt_fmt(u32_t code);
static int inst_exec_rsqrt1(u32_t code);
static int inst_exec_rsqrt2(u32_t code);
static int inst_exec_sb(u32_t code);
static int inst_exec_sc(u32_t code);
static int inst_exec_sdbbp(u32_t code);
static int inst_exec_sdc1(u32_t code);
static int inst_exec_sdc2(u32_t code);
static int inst_exec_sdxc1(u32_t code);
static int inst_exec_seb(u32_t code);
static int inst_exec_seh(u32_t code);
static int inst_exec_sh(u32_t code);
static int inst_exec_sll(u32_t code);
static int inst_exec_sllv(u32_t code);
static int inst_exec_slt(u32_t code);
static int inst_exec_slti(u32_t code);
static int inst_exec_sltiu(u32_t code);
static int inst_exec_sltu(u32_t code);
static int inst_exec_sqrt_fmt(u32_t code);
static int inst_exec_sra(u32_t code);
static int inst_exec_srav(u32_t code);
static int inst_exec_srl(u32_t code);
static int inst_exec_srlv(u32_t code);
static int inst_exec_sub(u32_t code);
static int inst_exec_subu(u32_t code);
static int inst_exec_sub_fmt(u32_t code);
static int inst_exec_suxc1(u32_t code);
static int inst_exec_sw(u32_t code);
static int inst_exec_swc1(u32_t code);
static int inst_exec_swc2(u32_t code);
static int inst_exec_swl(u32_t code);
static int inst_exec_swr(u32_t code);
static int inst_exec_swxc1(u32_t code);
static int inst_exec_sync(u32_t code);
static int inst_exec_synci(u32_t code);
static int inst_exec_syscall(u32_t code);
static int inst_exec_teq(u32_t code);
static int inst_exec_teqi(u32_t code);
static int inst_exec_tge(u32_t code);
static int inst_exec_tgei(u32_t code);
static int inst_exec_tgeiu(u32_t code);
static int inst_exec_tgeu(u32_t code);
static int inst_exec_tlbp(u32_t code);
static int inst_exec_tlbr(u32_t code);
static int inst_exec_tlbwi(u32_t code);
static int inst_exec_tlbwr(u32_t code);
static int inst_exec_tlt(u32_t code);
static int inst_exec_tlti(u32_t code);
static int inst_exec_tltiu(u32_t code);
static int inst_exec_tltu(u32_t code);
static int inst_exec_tne(u32_t code);
static int inst_exec_tnei(u32_t code);
static int inst_exec_trunc_l_fmt(u32_t code);
static int inst_exec_trunc_w_fmt(u32_t code);
static int inst_exec_wait(u32_t code);
static int inst_exec_wrpgpr(u32_t code);
static int inst_exec_wsbh(u32_t code);
static int inst_exec_xor(u32_t code);
static int inst_exec_xori(u32_t code);

static int (* const inst_callback[])(u32_t) = 
{
    [INST_NONE]         = inst_not_implemented,
    [INST_ABS_FMT]      = inst_not_implemented,
    [INST_ADD]          = inst_exec_add,
    [INST_ADDI]         = inst_exec_addi,
    [INST_ADDIU]        = inst_exec_addiu,
    [INST_ADDR_PS]      = inst_not_implemented,
    [INST_ADDU]         = inst_exec_addu,
    [INST_ADD_FMT]      = inst_not_implemented,
    [INST_ALNV_PS]      = inst_not_implemented,
    [INST_AND]          = inst_exec_and,
    [INST_ANDI]         = inst_exec_andi,
    [INST_BC1]          = inst_not_implemented,
    [INST_BC1ANY2]      = inst_not_implemented,
    [INST_BC1ANY4]      = inst_not_implemented,
    [INST_BC2]          = inst_not_implemented,
    [INST_BEQ]          = inst_exec_beq,
    [INST_BEQL]         = inst_not_implemented,
    [INST_BGEZ]         = inst_exec_bgez,
    [INST_BGEZAL]       = inst_exec_bgezal,
    [INST_BGEZALL]      = inst_not_implemented,
    [INST_BGEZL]        = inst_not_implemented,
    [INST_BGTZ]         = inst_not_implemented,
    [INST_BGTZL]        = inst_not_implemented,
    [INST_BLEZ]         = inst_not_implemented,
    [INST_BLEZL]        = inst_not_implemented,
    [INST_BLTZ]         = inst_exec_bltz,
    [INST_BLTZAL]       = inst_not_implemented,
    [INST_BLTZALL]      = inst_not_implemented,
    [INST_BLTZL]        = inst_not_implemented,
    [INST_BNE]          = inst_exec_bne,
    [INST_BNEL]         = inst_not_implemented,
    [INST_BREAK]        = inst_not_implemented,
    [INST_CACHE]        = inst_not_implemented,
    [INST_CEIL_L_FMT]   = inst_not_implemented,
    [INST_CEIL_W_FMT]   = inst_not_implemented,
    [INST_CFC1]         = inst_not_implemented,
    [INST_CFC2]         = inst_not_implemented,
    [INST_CLO]          = inst_not_implemented,
    [INST_CLZ]          = inst_not_implemented,
    [INST_CTC1]         = inst_not_implemented,
    [INST_CTC2]         = inst_not_implemented,
    [INST_CVT_D_FMT]    = inst_not_implemented,
    [INST_CVT_L_FMT]    = inst_not_implemented,
    [INST_CVT_PS_PW]    = inst_not_implemented,
    [INST_CVT_PS_S]     = inst_not_implemented,
    [INST_CVT_PW_PS]    = inst_not_implemented,
    [INST_CVT_S_FMT]    = inst_not_implemented,
    [INST_CVT_S_PL]     = inst_not_implemented,
    [INST_CVT_S_PU]     = inst_not_implemented,
    [INST_CVT_W_FMT]    = inst_not_implemented,
    [INST_C_EQ_FMT]     = inst_not_implemented,
    [INST_C_F_FMT]      = inst_not_implemented,
    [INST_C_LE_FMT]     = inst_not_implemented,
    [INST_C_LT_FMT]     = inst_not_implemented,
    [INST_C_NGE_FMT]    = inst_not_implemented,
    [INST_C_NGL_FMT]    = inst_not_implemented,
    [INST_C_NGLE_FMT]   = inst_not_implemented,
    [INST_C_NGT_FMT]    = inst_not_implemented,
    [INST_C_OLE_FMT]    = inst_not_implemented,
    [INST_C_OLT_FMT]    = inst_not_implemented,
    [INST_C_SEQ_FMT]    = inst_not_implemented,
    [INST_C_SF_FMT]     = inst_not_implemented,
    [INST_C_UEQ_FMT]    = inst_not_implemented,
    [INST_C_ULE_FMT]    = inst_not_implemented,
    [INST_C_ULT_FMT]    = inst_not_implemented,
    [INST_C_UN_FMT]     = inst_not_implemented,
    [INST_DERET]        = inst_not_implemented,
    [INST_DIV]          = inst_exec_div,
    [INST_DIVU]         = inst_exec_divu,
    [INST_DIV_FMT]      = inst_not_implemented,
    [INST_ERET]         = inst_exec_eret,
    [INST_EXT]          = inst_not_implemented,
    [INST_FLOOR_L_FMT]  = inst_not_implemented,
    [INST_FLOOR_W_FMT]  = inst_not_implemented,
    [INST_INS]          = inst_not_implemented,
    [INST_J]            = inst_exec_j,
    [INST_JAL]          = inst_exec_jal,
    [INST_JALR]         = inst_exec_jalr,
    [INST_JALX]         = inst_not_implemented,
    [INST_JR]           = inst_exec_jr,
    [INST_LB]           = inst_exec_lb,
    [INST_LBU]          = inst_not_implemented,
    [INST_LDC1]         = inst_not_implemented,
    [INST_LDC2]         = inst_not_implemented,
    [INST_LDXC1]        = inst_not_implemented,
    [INST_LH]           = inst_not_implemented,
    [INST_LHU]          = inst_not_implemented,
    [INST_LL]           = inst_not_implemented,
    [INST_LUI]          = inst_exec_lui,
    [INST_LUXC1]        = inst_not_implemented,
    [INST_LW]           = inst_exec_lw,
    [INST_LWC1]         = inst_not_implemented,
    [INST_LWC2]         = inst_not_implemented,
    [INST_LWL]          = inst_not_implemented,
    [INST_LWR]          = inst_not_implemented,
    [INST_LWXC1]        = inst_not_implemented,
    [INST_MADD]         = inst_not_implemented,
    [INST_MADDU]        = inst_not_implemented,
    [INST_MADD_D]       = inst_not_implemented,
    [INST_MADD_PS]      = inst_not_implemented,
    [INST_MADD_S]       = inst_not_implemented,
    [INST_MFC0]         = inst_exec_mfc0,
    [INST_MFC1]         = inst_not_implemented,
    [INST_MFC2]         = inst_not_implemented,
    [INST_MFHC1]        = inst_not_implemented,
    [INST_MFHC2]        = inst_not_implemented,
    [INST_MFHI]         = inst_exec_mfhi,
    [INST_MFLO]         = inst_exec_mflo,
    [INST_MFMC0]        = inst_not_implemented,
    [INST_MOVF]         = inst_not_implemented,
    [INST_MOVF_FMT]     = inst_not_implemented,
    [INST_MOVN]         = inst_not_implemented,
    [INST_MOVN_FMT]     = inst_not_implemented,
    [INST_MOVT]         = inst_not_implemented,
    [INST_MOVT_FMT]     = inst_not_implemented,
    [INST_MOVZ]         = inst_not_implemented,
    [INST_MOVZ_FMT]     = inst_not_implemented,
    [INST_MOV_FMT]      = inst_not_implemented,
    [INST_MSUB]         = inst_not_implemented,
    [INST_MSUBU]        = inst_not_implemented,
    [INST_MSUB_D]       = inst_not_implemented,
    [INST_MSUB_PS]      = inst_not_implemented,
    [INST_MSUB_S]       = inst_not_implemented,
    [INST_MTC0]         = inst_exec_mtc0,
    [INST_MTC1]         = inst_not_implemented,
    [INST_MTC2]         = inst_not_implemented,
    [INST_MTHC1]        = inst_not_implemented,
    [INST_MTHC2]        = inst_not_implemented,
    [INST_MTHI]         = inst_exec_mthi,
    [INST_MTLO]         = inst_exec_mtlo,
    [INST_MUL]          = inst_not_implemented,
    [INST_MULR_PS]      = inst_not_implemented,
    [INST_MULT]         = inst_exec_mult,
    [INST_MULTU]        = inst_exec_multu,
    [INST_MUL_FMT]      = inst_not_implemented,
    [INST_NEG_FMT]      = inst_not_implemented,
    [INST_NMADD_D]      = inst_not_implemented,
    [INST_NMADD_PS]     = inst_not_implemented,
    [INST_NMADD_S]      = inst_not_implemented,
    [INST_NMSUB_D]      = inst_not_implemented,
    [INST_NMSUB_PS]     = inst_not_implemented,
    [INST_NMSUB_S]      = inst_not_implemented,
    [INST_NOR]          = inst_exec_nor,
    [INST_OR]           = inst_exec_or,
    [INST_ORI]          = inst_exec_ori,
    [INST_PLL_PS]       = inst_not_implemented,
    [INST_PLU_PS]       = inst_not_implemented,
    [INST_PREF]         = inst_not_implemented,
    [INST_PREFX]        = inst_not_implemented,
    [INST_PUL_PS]       = inst_not_implemented,
    [INST_PUU_PS]       = inst_not_implemented,
    [INST_RDHWR]        = inst_not_implemented,
    [INST_RDPGPR]       = inst_exec_rdpgpr,
    [INST_RECIP_FMT]    = inst_not_implemented,
    [INST_RECIP1]       = inst_not_implemented,
    [INST_RECIP2]       = inst_not_implemented,
    [INST_ROTR]         = inst_not_implemented,
    [INST_ROTRV]        = inst_not_implemented,
    [INST_ROUND_L_FMT]  = inst_not_implemented,
    [INST_ROUND_W_FMT]  = inst_not_implemented,
    [INST_RSQRT_FMT]    = inst_not_implemented,
    [INST_RSQRT1]       = inst_not_implemented,
    [INST_RSQRT2]       = inst_not_implemented,
    [INST_SB]           = inst_exec_sb,
    [INST_SC]           = inst_not_implemented,
    [INST_SDBBP]        = inst_not_implemented,
    [INST_SDC1]         = inst_not_implemented,
    [INST_SDC2]         = inst_not_implemented,
    [INST_SDXC1]        = inst_not_implemented,
    [INST_SEB]          = inst_not_implemented,
    [INST_SEH]          = inst_not_implemented,
    [INST_SH]           = inst_not_implemented,
    [INST_SLL]          = inst_exec_sll,
    [INST_SLLV]         = inst_exec_sllv,
    [INST_SLT]          = inst_exec_slt,
    [INST_SLTI]         = inst_exec_slti,
    [INST_SLTIU]        = inst_exec_sltiu,
    [INST_SLTU]         = inst_exec_sltu,
    [INST_SQRT_FMT]     = inst_not_implemented,
    [INST_SRA]          = inst_not_implemented,
    [INST_SRAV]         = inst_not_implemented,
    [INST_SRL]          = inst_exec_srl,
    [INST_SRLV]         = inst_exec_srlv,
    [INST_SUB]          = inst_exec_sub,
    [INST_SUBU]         = inst_exec_subu,
    [INST_SUB_FMT]      = inst_not_implemented,
    [INST_SUXC1]        = inst_not_implemented,
    [INST_SW]           = inst_exec_sw,
    [INST_SWC1]         = inst_not_implemented,
    [INST_SWC2]         = inst_not_implemented,
    [INST_SWL]          = inst_not_implemented,
    [INST_SWR]          = inst_not_implemented,
    [INST_SWXC1]        = inst_not_implemented,
    [INST_SYNC]         = inst_exec_sync,
    [INST_SYNCI]        = inst_not_implemented,
    [INST_SYSCALL]      = inst_not_implemented,
    [INST_TEQ]          = inst_not_implemented,
    [INST_TEQI]         = inst_not_implemented,
    [INST_TGE]          = inst_not_implemented,
    [INST_TGEI]         = inst_not_implemented,
    [INST_TGEIU]        = inst_not_implemented,
    [INST_TGEU]         = inst_not_implemented,
    [INST_TLBP]         = inst_not_implemented,
    [INST_TLBR]         = inst_not_implemented,
    [INST_TLBWI]        = inst_not_implemented,
    [INST_TLBWR]        = inst_not_implemented,
    [INST_TLT]          = inst_not_implemented,
    [INST_TLTI]         = inst_not_implemented,
    [INST_TLTIU]        = inst_not_implemented,
    [INST_TLTU]         = inst_not_implemented,
    [INST_TNE]          = inst_not_implemented,
    [INST_TNEI]         = inst_not_implemented,
    [INST_TRUNC_L_FMT]  = inst_not_implemented,
    [INST_TRUNC_W_FMT]  = inst_not_implemented,
    [INST_WAIT]         = inst_not_implemented,
    [INST_WRPGPR]       = inst_exec_wrpgpr,
    [INST_WSBH]         = inst_not_implemented,
    [INST_XOR]          = inst_exec_xor,
    [INST_XORI]         = inst_exec_xori,
};

static inline void branch_to_offset(int offset)
{
    u32_t pcdata;
    reg_special_read(REG_SPECIAL_PC, &pcdata);
    pcdata += offset * 4 + 4;
    reg_special_write(REG_SPECIAL_PC_ADVANCE2, pcdata);
    reg_special_write(REG_SPECIAL_BRANCH_JUMP, MEMU_TRUE);
#if DUMP_INST
    fprintf(LOG_FILE, "Branch: offset=%d, target=%.8X\n", offset, pcdata);
#endif
    return;
}

static inline void jump_to_target(u32_t target)
{
    reg_special_write(REG_SPECIAL_PC_ADVANCE2, target);
    reg_special_write(REG_SPECIAL_BRANCH_JUMP, MEMU_TRUE);
#if DUMP_INST
    fprintf(LOG_FILE, "Jump: target=%.8X\n", target);
#endif
    return;
}

static inline void link_pc_ra()
{
    u32_t pcdata;
    reg_special_read(REG_SPECIAL_PC, &pcdata);
    reg_gpr_write(REG_GPR_RA, pcdata + 8);
#if DUMP_INST
    fprintf(LOG_FILE, "Link: ra=%.8X\n", pcdata + 8);
    //fprintf(LOG_FILE, "Link: ra=%.8X\n", pcdata + 4);
#endif
    return;
}

static int inst_not_implemented(u32_t code)
{
#if DUMP_INST
    fprintf(LOG_FILE, "memu: error: instruction %.8X invalid or not implemented\n", code);
#endif
    return EXCEPTION_RESERVED_INSTRUCTION;
}

static int inst_exec_abs_fmt(u32_t code);

static int inst_exec_add(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 + regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ADD: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_addi(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHRSIGNEXT(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    regdata += immediate;
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ADDI: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_addiu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHRSIGNEXT(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    regdata += immediate;
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ADDIU: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_addr_ps(u32_t code);

static int inst_exec_addu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 + regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ADDU: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_add_fmt(u32_t code);
static int inst_exec_alnv_ps(u32_t code);

static int inst_exec_and(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 & regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: AND: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_andi(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHR(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    regdata &= immediate;
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ANDI: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_bc1(u32_t code);
static int inst_exec_bc1any2(u32_t code);
static int inst_exec_bc1any4(u32_t code);
static int inst_exec_bc2(u32_t code);

static int inst_exec_beq(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    i32_t r1;
    i32_t r2;
    reg_gpr_read(rs, &r1);
    reg_gpr_read(rt, &r2);
    if (r1 == r2)
        branch_to_offset(offset);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: BEQ: rs=%d, rt=%d, offset=%d\n", rs, rt, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_beql(u32_t code);

static int inst_exec_bgez(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    if (regdata >= 0)
        branch_to_offset(offset);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: BGEZ: rs=%d, offset=%d\n", rs, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_bgezal(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    link_pc_ra();
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    if (regdata >= 0)
        branch_to_offset(offset);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: BGEZAL: rs=%d, offset=%d\n", rs, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_bgezall(u32_t code);
static int inst_exec_bgezl(u32_t code);
static int inst_exec_bgtz(u32_t code);
static int inst_exec_bgtzl(u32_t code);
static int inst_exec_blez(u32_t code);
static int inst_exec_blezl(u32_t code);

static int inst_exec_bltz(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    if (regdata < 0)
        branch_to_offset(offset);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: BLTZ: rs=%d, offset=%d\n", rs, offset);    
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_bltzal(u32_t code);
static int inst_exec_bltzall(u32_t code);
static int inst_exec_bltzl(u32_t code);

static int inst_exec_bne(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    i32_t r1;
    i32_t r2;
    reg_gpr_read(rs, &r1);
    reg_gpr_read(rt, &r2);
    if (r1 != r2)
        branch_to_offset(offset);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: BNE: rs=%d, rt=%d, offset=%d\n", rs, rt, offset);
    fprintf(LOG_FILE, "r1=%d, r2=%d\n", r1, r2);
#endif
    return EXCEPTION_NONE; 
}

static int inst_exec_bnel(u32_t code);
static int inst_exec_break(u32_t code);
static int inst_exec_cache(u32_t code);
static int inst_exec_ceil_l_fmt(u32_t code);
static int inst_exec_ceil_w_fmt(u32_t code);
static int inst_exec_cfc1(u32_t code);
static int inst_exec_cfc2(u32_t code);
static int inst_exec_clo(u32_t code);
static int inst_exec_clz(u32_t code);
static int inst_exec_ctc1(u32_t code);
static int inst_exec_ctc2(u32_t code);
static int inst_exec_cvt_d_fmt(u32_t code);
static int inst_exec_cvt_l_fmt(u32_t code);
static int inst_exec_cvt_ps_pw(u32_t code);
static int inst_exec_cvt_ps_s(u32_t code);
static int inst_exec_cvt_pw_ps(u32_t code);
static int inst_exec_cvt_s_fmt(u32_t code);
static int inst_exec_cvt_s_pl(u32_t code);
static int inst_exec_cvt_s_pu(u32_t code);
static int inst_exec_cvt_w_fmt(u32_t code);
static int inst_exec_c_eq_fmt(u32_t code);
static int inst_exec_c_f_fmt(u32_t code);
static int inst_exec_c_le_fmt(u32_t code);
static int inst_exec_c_lt_fmt(u32_t code);
static int inst_exec_c_nge_fmt(u32_t code);
static int inst_exec_c_ngl_fmt(u32_t code);
static int inst_exec_c_ngle_fmt(u32_t code);
static int inst_exec_c_ngt_fmt(u32_t code);
static int inst_exec_c_ole_fmt(u32_t code);
static int inst_exec_c_olt_fmt(u32_t code);
static int inst_exec_c_seq_fmt(u32_t code);
static int inst_exec_c_sf_fmt(u32_t code);
static int inst_exec_c_ueq_fmt(u32_t code);
static int inst_exec_c_ule_fmt(u32_t code);
static int inst_exec_c_ult_fmt(u32_t code);
static int inst_exec_c_un_fmt(u32_t code);
static int inst_exec_deret(u32_t code);

static int inst_exec_div(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    i32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    i32_t regdata2; 
    reg_gpr_read(rt, &regdata2);
    if (regdata2)
    {
        reg_special_write(REG_SPECIAL_HI, regdata1 / regdata2);
        reg_special_write(REG_SPECIAL_LO, regdata1 % regdata2);
    }
    else
    {
#if DUMP_INST
        fprintf(LOG_FILE, "Instruction: DIV: reg[rt]=0\n");
#endif
    }
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: DIV: rs=%d, rt=%d\n", rs, rt);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_divu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    u32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    u32_t regdata2; 
    reg_gpr_read(rt, &regdata2);
    if (regdata2)
    {
        reg_special_write(REG_SPECIAL_HI, regdata1 / regdata2);
        reg_special_write(REG_SPECIAL_LO, regdata1 % regdata2);
    }
    else
    {
#if DUMP_INST
        fprintf(LOG_FILE, "Instruction: DIVU: reg[rt]=0\n");
#endif
    }
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: DIVU: rs=%d, rt=%d\n", rs, rt);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_div_fmt(u32_t code);

static int inst_exec_eret(u32_t code)
{
    u32_t exl;
    reg_cpr_read(FKREG_CPR_EXL, 0, &exl);   //EXL bit
    if (!exl)
    {
#if DUMP_INTERRUPT
        fprintf(LOG_FILE, "Interrupt: ERET failed: EXL=%d\n", exl);
#endif
        //TODO: handle this.
        return EXCEPTION_NONE; 
    }
    u32_t epc;
    reg_cpr_read(FKREG_CPR_EPC, 0, &epc);
    reg_special_write(REG_SPECIAL_PC_ADVANCE1, epc);
    reg_special_write(REG_SPECIAL_PC_ADVANCE2, epc + 4);
    reg_cpr_write(FKREG_CPR_EXL, 0, MEMU_FALSE);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ERET: EOC=%.8X\n", epc);
#endif 
    return EXCEPTION_NONE;
}

static int inst_exec_ext(u32_t code);
static int inst_exec_floor_l_fmt(u32_t code);
static int inst_exec_floor_w_fmt(u32_t code);
static int inst_exec_ins(u32_t code);

static int inst_exec_j(u32_t code)
{
    int index = MASKSHR(code, 25, 0);
    u32_t pcdata;
    reg_special_read(REG_SPECIAL_PC, &pcdata);
    u32_t target = MASK(pcdata, 31, 28) | (index << 2);
    jump_to_target(target);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: J: index=%d\n", index);
#endif 
    return EXCEPTION_NONE;
}

static int inst_exec_jal(u32_t code)
{
    int index = MASKSHR(code, 25, 0);
    link_pc_ra();
    u32_t pcdata;
    reg_special_read(REG_SPECIAL_PC, &pcdata);
    u32_t target = MASK(pcdata, 31, 28) | (index << 2);
    jump_to_target(target);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: JAL: index=%d\n", index);
#endif 
    return EXCEPTION_NONE;
}

static int inst_exec_jalr(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int hint = MASKSHR(code, 10, 6);
    link_pc_ra();
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    jump_to_target(regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: JALR: rs=%d, hint=%d\n", rs, hint);
#endif 
    return EXCEPTION_NONE;
}

static int inst_exec_jalx(u32_t code);

static int inst_exec_jr(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int hint = MASKSHR(code, 10, 6);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    jump_to_target(regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: JR: rs=%d, hint=%d\n", rs, hint);
#endif 
    return EXCEPTION_NONE;
}

static int inst_exec_lb(u32_t code)
{
    int base = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    u32_t vaddr;
    reg_gpr_read(base, &vaddr);
    vaddr += offset;
    u32_t paddr;
    u32_t attr;
    int access_type = MEM_ACCESS_LEN_1 | MEM_ACCESS_DATA | MEM_ACCESS_READ;
    i32_t memdata;
    int exception = mmu_addr_trans(vaddr, access_type, &paddr, &attr);
    if (exception != EXCEPTION_NONE)
        return exception;
    exception = mem_read(paddr, vaddr, attr, access_type, &memdata);
    if (exception != EXCEPTION_NONE)
        return exception;
    reg_gpr_write(rt, memdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: LB: base=%d, rt=%d, offset=%d\n", base, rt, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_lbu(u32_t code);
static int inst_exec_ldc1(u32_t code);
static int inst_exec_ldc2(u32_t code);
static int inst_exec_ldxc1(u32_t code);
static int inst_exec_lh(u32_t code);
static int inst_exec_lhu(u32_t code);
static int inst_exec_ll(u32_t code);

static int inst_exec_lui(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHR(code, 15, 0);
    reg_gpr_write(rt, immediate << 16);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: LUI: rt=%d, immediate=%d\n", rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_luxc1(u32_t code);

static int inst_exec_lw(u32_t code)
{
    int base = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    u32_t vaddr;
    reg_gpr_read(base, &vaddr);
    vaddr += offset;
    u32_t paddr;
    u32_t attr;
    int access_type = MEM_ACCESS_LEN_4 | MEM_ACCESS_DATA | MEM_ACCESS_READ;
    i32_t memdata;
    int exception = mmu_addr_trans(vaddr, access_type, &paddr, &attr);
    if (exception != EXCEPTION_NONE)
        return exception;
    exception = mem_read(paddr, vaddr, attr, access_type, &memdata);
    if (exception != EXCEPTION_NONE)
        return exception;
    reg_gpr_write(rt, memdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: LW: base=%d, rt=%d, offset=%d\n", base, rt, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_lwc1(u32_t code);
static int inst_exec_lwc2(u32_t code);
static int inst_exec_lwl(u32_t code);
static int inst_exec_lwr(u32_t code);
static int inst_exec_lwxc1(u32_t code);
static int inst_exec_madd(u32_t code);
static int inst_exec_maddu(u32_t code);
static int inst_exec_madd_d(u32_t code);
static int inst_exec_madd_ps(u32_t code);
static int inst_exec_madd_s(u32_t code);

static int inst_exec_mfc0(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    int sel = MASKSHR(code, 2, 0);
    i32_t regdata;
    reg_cpr_read(rd, sel, &regdata);
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MFC0: rt=%d, rd=%d, sel=%d\n", rt, rd, sel);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mfc1(u32_t code);
static int inst_exec_mfc2(u32_t code);
static int inst_exec_mfhc1(u32_t code);
static int inst_exec_mfhc2(u32_t code);

static int inst_exec_mfhi(u32_t code)
{
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata;
    reg_special_read(REG_SPECIAL_HI, &regdata);
    reg_gpr_write(rd, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MFHI: rd=%d, word=%08X\n", rd, regdata);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mflo(u32_t code)
{
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata;
    reg_special_read(REG_SPECIAL_LO, &regdata);
    reg_gpr_write(rd, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MFLO: rd=%d\n", rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mfmc0(u32_t code);
static int inst_exec_movf(u32_t code);
static int inst_exec_movf_fmt(u32_t code);
static int inst_exec_movn(u32_t code);
static int inst_exec_movn_fmt(u32_t code);
static int inst_exec_movt(u32_t code);
static int inst_exec_movt_fmt(u32_t code);
static int inst_exec_movz(u32_t code);
static int inst_exec_movz_fmt(u32_t code);
static int inst_exec_mov_fmt(u32_t code);
static int inst_exec_msub(u32_t code);
static int inst_exec_msubu(u32_t code);
static int inst_exec_msub_d(u32_t code);
static int inst_exec_msub_ps(u32_t code);
static int inst_exec_msub_s(u32_t code);

static int inst_exec_mtc0(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    int sel = MASKSHR(code, 2, 0);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    reg_cpr_write(rd, sel, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MTC0: rt=%d, rd=%d, sel=%d\n", rt, rd, sel);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mtc1(u32_t code);
static int inst_exec_mtc2(u32_t code);
static int inst_exec_mthc1(u32_t code);
static int inst_exec_mthc2(u32_t code);

static int inst_exec_mthi(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    reg_special_write(REG_SPECIAL_HI, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MTHI: rs=%d\n", rs);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mtlo(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    reg_special_write(REG_SPECIAL_LO, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MTLO: rs=%d\n", rs);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mul(u32_t code);
static int inst_exec_mulr_ps(u32_t code);

static int inst_exec_mult(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    i32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    i32_t regdata2; 
    reg_gpr_read(rt, &regdata2);
    i64_t regdata = (u64_t)regdata1 * (u64_t)regdata2;
    reg_special_write(REG_SPECIAL_HI, MASK64SHR(regdata, 63, 32));
    reg_special_write(REG_SPECIAL_LO, MASK64SHR(regdata, 31, 0));
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MULT: rs=%d, rt=%d\n", rs, rt);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_multu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    u32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    u32_t regdata2; 
    reg_gpr_read(rt, &regdata2);
    u64_t regdata = (u64_t)regdata1 * (u64_t)regdata2;
    reg_special_write(REG_SPECIAL_HI, MASK64SHR(regdata, 63, 32));
    reg_special_write(REG_SPECIAL_LO, MASK64SHR(regdata, 31, 0));
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: MULTU: rs=%d, rt=%d\n", rs, rt);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_mul_fmt(u32_t code);
static int inst_exec_neg_fmt(u32_t code);
static int inst_exec_nmadd_d(u32_t code);
static int inst_exec_nmadd_ps(u32_t code);
static int inst_exec_nmadd_s(u32_t code);
static int inst_exec_nmsub_d(u32_t code);
static int inst_exec_nmsub_ps(u32_t code);
static int inst_exec_nmsub_s(u32_t code);

static int inst_exec_nor(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, ~(regdata1 | regdata2));
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: NOR: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_or(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 | regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: OR: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_ori(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHR(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    regdata |= immediate;
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: ORI: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_pll_ps(u32_t code);
static int inst_exec_plu_ps(u32_t code);
static int inst_exec_pref(u32_t code);
static int inst_exec_prefx(u32_t code);
static int inst_exec_pul_ps(u32_t code);
static int inst_exec_puu_ps(u32_t code);
static int inst_exec_rdhwr(u32_t code);

static int inst_exec_rdpgpr(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    reg_gpr_shadow_read(rd, rt);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: RDPGPR: rt=%d, rd=%d\n", rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_recip_fmt(u32_t code);
static int inst_exec_recip1(u32_t code);
static int inst_exec_recip2(u32_t code);
static int inst_exec_rotr(u32_t code);
static int inst_exec_rotrv(u32_t code);
static int inst_exec_round_l_fmt(u32_t code);
static int inst_exec_round_w_fmt(u32_t code);
static int inst_exec_rsqrt_fmt(u32_t code);
static int inst_exec_rsqrt1(u32_t code);
static int inst_exec_rsqrt2(u32_t code);

static int inst_exec_sb(u32_t code)
{
    int base = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    u32_t vaddr;
    reg_gpr_read(base, &vaddr);
    vaddr += offset;
    u32_t paddr;
    u32_t attr;
    int access_type = MEM_ACCESS_LEN_1 | MEM_ACCESS_DATA | MEM_ACCESS_WRITE;
    int exception = mmu_addr_trans(vaddr, access_type, &paddr, &attr);
    if (exception != EXCEPTION_NONE)
        return exception;
    i32_t memdata;
    reg_gpr_read(rt, &memdata);
    exception = mem_write(paddr, vaddr, attr, access_type, memdata);
    if (exception != EXCEPTION_NONE)
        return exception;
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SB: base=%d, rt=%d, offset=%d\n", base, rt, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_sc(u32_t code);
static int inst_exec_sdbbp(u32_t code);
static int inst_exec_sdc1(u32_t code);
static int inst_exec_sdc2(u32_t code);
static int inst_exec_sdxc1(u32_t code);
static int inst_exec_seb(u32_t code);
static int inst_exec_seh(u32_t code);
static int inst_exec_sh(u32_t code);

static int inst_exec_sll(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    int sa = MASKSHR(code, 10, 6);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    reg_gpr_write(rd, regdata << sa);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLL: rt=%d, rd=%d, sa=%d\n", rt, rd, sa);
#endif    
    return EXCEPTION_NONE;
}

static int inst_exec_sllv(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    i32_t sa;
    reg_gpr_read(rs, &sa);
    reg_gpr_write(rd, regdata << sa);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLLV: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif    
    return EXCEPTION_NONE;
}

static int inst_exec_slt(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    i32_t regdata2;
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 < regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLT: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif    
    return EXCEPTION_NONE;
}

static int inst_exec_slti(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHRSIGNEXT(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    reg_gpr_write(rt, *(u32_t*)&regdata < *(u32_t*)&immediate);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLTI: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_sltiu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHR(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    reg_gpr_write(rt, *(u32_t*)&regdata < *(u32_t*)&immediate);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLTIU: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_sltu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    u32_t regdata1;
    reg_gpr_read(rs, &regdata1);
    u32_t regdata2;
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 < regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SLTU: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif    
    return EXCEPTION_NONE;
}

static int inst_exec_sqrt_fmt(u32_t code);

static int inst_exec_sra(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    int sa = MASKSHR(code, 10, 6);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    regdata >>= sa;
    reg_gpr_write(rd, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SRA: rt=%d, rd=%d, sa=%d\n", rt, rd, sa);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_srav(u32_t code);

static int inst_exec_srl(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    int sa = MASKSHR(code, 10, 6);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    *(u32_t*)&regdata >>= sa;
    reg_gpr_write(rd, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SRL: rt=%d, rd=%d, sa=%d\n", rt, rd, sa);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_srlv(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata;
    reg_gpr_read(rt, &regdata);
    i32_t sa;
    reg_gpr_read(rs, &sa);
    *(u32_t*)&regdata >>= sa;
    reg_gpr_write(rd, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SRLV: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_sub(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 - regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SUB: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_subu(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 - regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SUBU: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}


static int inst_exec_sub_fmt(u32_t code);
static int inst_exec_suxc1(u32_t code);

static int inst_exec_sw(u32_t code)
{
    int base = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int offset = MASKSHRSIGNEXT(code, 15, 0);
    u32_t vaddr;
    reg_gpr_read(base, &vaddr);
    vaddr += offset;
    u32_t paddr;
    u32_t attr;
    int access_type = MEM_ACCESS_LEN_4 | MEM_ACCESS_DATA | MEM_ACCESS_WRITE;
    int exception = mmu_addr_trans(vaddr, access_type, &paddr, &attr);
    if (exception != EXCEPTION_NONE)
        return exception;
    i32_t memdata;
    reg_gpr_read(rt, &memdata);
    exception = mem_write(paddr, vaddr, attr, access_type, memdata);
    if (exception != EXCEPTION_NONE)
        return exception;
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SW: base=%d, rt=%d, offset=%d\n", base, rt, offset);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_swc1(u32_t code);
static int inst_exec_swc2(u32_t code);
static int inst_exec_swl(u32_t code);
static int inst_exec_swr(u32_t code);
static int inst_exec_swxc1(u32_t code);

static int inst_exec_sync(u32_t code)
{
    int stype = MASKSHR(code, 10, 6);
    /* nothing to do here */
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: SYNC: stype=%d\n", stype);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_synci(u32_t code);
static int inst_exec_syscall(u32_t code);
static int inst_exec_teq(u32_t code);
static int inst_exec_teqi(u32_t code);
static int inst_exec_tge(u32_t code);
static int inst_exec_tgei(u32_t code);
static int inst_exec_tgeiu(u32_t code);
static int inst_exec_tgeu(u32_t code);
static int inst_exec_tlbp(u32_t code);
static int inst_exec_tlbr(u32_t code);
static int inst_exec_tlbwi(u32_t code);
static int inst_exec_tlbwr(u32_t code);
static int inst_exec_tlt(u32_t code);
static int inst_exec_tlti(u32_t code);
static int inst_exec_tltiu(u32_t code);
static int inst_exec_tltu(u32_t code);
static int inst_exec_tne(u32_t code);
static int inst_exec_tnei(u32_t code);
static int inst_exec_trunc_l_fmt(u32_t code);
static int inst_exec_trunc_w_fmt(u32_t code);
static int inst_exec_wait(u32_t code);

static int inst_exec_wrpgpr(u32_t code)
{
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    reg_gpr_shadow_write(rd, rt);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: WRPGPR: rt=%d, rd=%d\n", rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_wsbh(u32_t code);

static int inst_exec_xor(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int rd = MASKSHR(code, 15, 11);
    i32_t regdata1;
    i32_t regdata2;
    reg_gpr_read(rs, &regdata1);
    reg_gpr_read(rt, &regdata2);
    reg_gpr_write(rd, regdata1 ^ regdata2);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: XOR: rs=%d, rt=%d, rd=%d\n", rs, rt, rd);
#endif
    return EXCEPTION_NONE;
}

static int inst_exec_xori(u32_t code)
{
    int rs = MASKSHR(code, 25, 21);
    int rt = MASKSHR(code, 20, 16);
    int immediate = MASKSHR(code, 15, 0);
    i32_t regdata;
    reg_gpr_read(rs, &regdata);
    regdata ^= immediate;
    reg_gpr_write(rt, regdata);
#if DUMP_INST
    fprintf(LOG_FILE, "Instruction: XORI: rs=%d, rt=%d, immediate=%d\n", rs, rt, immediate);
#endif
    return EXCEPTION_NONE;
}

int inst_execute(u32_t code)
{
    u32_t inst = decode(code);
    if (MASK(inst, 31, 16)) /* instruction reserved or invalid */
        return EXCEPTION_RESERVED_INSTRUCTION;
    return inst_callback[inst](code);
}

