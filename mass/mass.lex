%{
/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#include "mass.tab.h"
#include "mass.h"
%}

%option noyywrap
%option yylineno

%%
 
 /* r-type instructions */
add     return yylval.int_t = INST_ADD;
addu    return yylval.int_t = INST_ADDU;
and     return yylval.int_t = INST_AND;
break   return yylval.int_t = INST_BREAK;
div     return yylval.int_t = INST_DIV;
divu    return yylval.int_t = INST_DIVU;
jalr    return yylval.int_t = INST_JALR;
jr      return yylval.int_t = INST_JR;
mfhi    return yylval.int_t = INST_MFHI;
mflo    return yylval.int_t = INST_MFLO;
mthi    return yylval.int_t = INST_MTHI;
mtlo    return yylval.int_t = INST_MTLO;
mult    return yylval.int_t = INST_MULT;
multu   return yylval.int_t = INST_MULTU;
nor     return yylval.int_t = INST_NOR;
or      return yylval.int_t = INST_OR;
sll     return yylval.int_t = INST_SLL;
sllv    return yylval.int_t = INST_SLLV;
slt     return yylval.int_t = INST_SLT;
sltu    return yylval.int_t = INST_SLTU;
sra     return yylval.int_t = INST_SRA;
srav    return yylval.int_t = INST_SRAV;
srl     return yylval.int_t = INST_SRL;
srlv    return yylval.int_t = INST_SRLV;
sub     return yylval.int_t = INST_SUB;
subu    return yylval.int_t = INST_SUBU;
syscall return yylval.int_t = INST_SYSCALL;
xor     return yylval.int_t = INST_XOR;

 /* i-type instructions */
addi    return yylval.int_t = INST_ADDI;
addiu   return yylval.int_t = INST_ADDIU;
andi    return yylval.int_t = INST_ANDI;
beq     return yylval.int_t = INST_BEQ;
bgez    return yylval.int_t = INST_BGEZ;
bgtz    return yylval.int_t = INST_BGTZ; 
blez    return yylval.int_t = INST_BLEZ;
bltz    return yylval.int_t = INST_BLTZ;
bne     return yylval.int_t = INST_BNE;
lb      return yylval.int_t = INST_LB;
lbu     return yylval.int_t = INST_LBU;
lh      return yylval.int_t = INST_LH;
lhu     return yylval.int_t = INST_LHU;
lui     return yylval.int_t = INST_LUI;
lw      return yylval.int_t = INST_LW;
lwcs    return yylval.int_t = INST_LWC1;
ori     return yylval.int_t = INST_ORI;
sb      return yylval.int_t = INST_SB;
slti    return yylval.int_t = INST_SLTI;
sltiu   return yylval.int_t = INST_SLTIU;
sh      return yylval.int_t = INST_SH;
sw      return yylval.int_t = INST_SW;
swcs    return yylval.int_t = INST_SWC1;
xori    return yylval.int_t = INST_XORI;

 /* j-type instructions */
j       return yylval.int_t = INST_J;
jal     return yylval.int_t = INST_JAL;

 /* cp0 instructions */
eret    return yylval.int_t = INST_ERET;
mfc0    return yylval.int_t = INST_MFC0;
mtc0    return yylval.int_t = INST_MTC0;
wait    return yylval.int_t = INST_WAIT;
rdpgpr  return yylval.int_t = INST_RDPGPR;
wrpgpr  return yylval.int_t = INST_WRPGPR;

 /* cp1 instructions */
add.s   return yylval.int_t = INST_ADD_S;
cvt.s.w return yylval.int_t = INST_CVT_S_W;
cvt.w.s return yylval.int_t = INST_CVT_W_S;
div.s   return yylval.int_t = INST_DIV_S;
mfc1    return yylval.int_t = INST_MFC1;
mov.s   return yylval.int_t = INST_MOV_S;
mtc1    return yylval.int_t = INST_MTC1;
mul.s   return yylval.int_t = INST_MUL_S;
sub.s   return yylval.int_t = INST_SUB_S;
 
 /* pseudo instructions */
move    return yylval.int_t = PSEUDO_INST_MOVE;
blt     return yylval.int_t = PSEUDO_INST_BLT;
nop     return yylval.int_t = PSEUDO_INST_NOP;
li      return yylval.int_t = PSEUDO_INST_LI; 
lla     return yylval.int_t = PSEUDO_INST_LLA; 

 /* registers */
$zero return yylval.int_t = REG_ZERO;
$at return yylval.int_t = REG_AT;
$v0 return yylval.int_t = REG_V0;
$v1 return yylval.int_t = REG_V1;
$a0 return yylval.int_t = REG_A0;
$a1 return yylval.int_t = REG_A1;
$a2 return yylval.int_t = REG_A2;
$a3 return yylval.int_t = REG_A3;
$t0 return yylval.int_t = REG_T0;
$t1 return yylval.int_t = REG_T1;
$t2 return yylval.int_t = REG_T2;
$t3 return yylval.int_t = REG_T3;
$t4 return yylval.int_t = REG_T4;
$t5 return yylval.int_t = REG_T5;
$t6 return yylval.int_t = REG_T6;
$t7 return yylval.int_t = REG_T7;
$s0 return yylval.int_t = REG_S0;
$s1 return yylval.int_t = REG_S1;
$s2 return yylval.int_t = REG_S2;
$s3 return yylval.int_t = REG_S3;
$s4 return yylval.int_t = REG_S4;
$s5 return yylval.int_t = REG_S5;
$s6 return yylval.int_t = REG_S6;
$s7 return yylval.int_t = REG_S7;
$t8 return yylval.int_t = REG_T8;
$t9 return yylval.int_t = REG_T9;
$k0 return yylval.int_t = REG_K0;
$k1 return yylval.int_t = REG_K1;
$gp return yylval.int_t = REG_GP;
$sp return yylval.int_t = REG_SP;
$fp return yylval.int_t = REG_FP;
$ra return yylval.int_t = REG_RA;
$0 return yylval.int_t = REG_ZERO;
$1 return yylval.int_t = REG_AT;
$2 return yylval.int_t = REG_V0;
$3 return yylval.int_t = REG_V1;
$4 return yylval.int_t = REG_A0;
$5 return yylval.int_t = REG_A1;
$6 return yylval.int_t = REG_A2;
$7 return yylval.int_t = REG_A3;
$8 return yylval.int_t = REG_T0;
$9 return yylval.int_t = REG_T1;
$10 return yylval.int_t = REG_T2;
$11 return yylval.int_t = REG_T3;
$12 return yylval.int_t = REG_T4;
$13 return yylval.int_t = REG_T5;
$14 return yylval.int_t = REG_T6;
$15 return yylval.int_t = REG_T7;
$16 return yylval.int_t = REG_S0;
$17 return yylval.int_t = REG_S1;
$18 return yylval.int_t = REG_S2;
$19 return yylval.int_t = REG_S3;
$20 return yylval.int_t = REG_S4;
$21 return yylval.int_t = REG_S5;
$22 return yylval.int_t = REG_S6;
$23 return yylval.int_t = REG_S7;
$24 return yylval.int_t = REG_T8;
$25 return yylval.int_t = REG_T9;
$26 return yylval.int_t = REG_K0;
$27 return yylval.int_t = REG_K1;
$28 return yylval.int_t = REG_GP;
$29 return yylval.int_t = REG_SP;
$30 return yylval.int_t = REG_FP;
$31 return yylval.int_t = REG_RA;

 /* operators for constant expressions */
\+  return yylval.int_t = OP_ADD;
\-  return yylval.int_t = OP_SUB;
\*  return yylval.int_t = OP_MUL;
\/  return yylval.int_t = OP_DIV;
\%  return yylval.int_t = OP_MOD;
\&  return yylval.int_t = OP_AND;
\|  return yylval.int_t = OP_OR;
\^  return yylval.int_t = OP_XOR;
\~  return yylval.int_t = OP_NOT;
\<\<    return yylval.int_t = OP_LSHIFT;
\>\>    return yylval.int_t = OP_RSHIFT;

 /* identifiers and numbers */
[_a-zA-Z][_0-9a-zA-Z]*  yylval.str_t = strdup(yytext); return IDENTIFIER;
[0-9]+      yylval.int_t = strtol(yytext, NULL, 10); return INTEGER;
0d[0-9]+    yylval.int_t = strtol(yytext, NULL, 10); return INTEGER;
0b[01]+     yylval.int_t = strtol(yytext, NULL, 2); return INTEGER;
0x[0-9a-zA-Z]+  yylval.int_t = strtol(yytext, NULL, 16); return INTEGER; 

 /* misc */
\(  return yylval.int_t = L_PARENTHESIS;
\)  return yylval.int_t = R_PARENTHESIS;
\,  return yylval.int_t = COMMA;
\.  return yylval.int_t = DOT;
\:  return yylval.int_t = COLON;  
\;  return yylval.int_t = SEMICOLON;
\.dup return yylval.int_t = AD_DUP;
\.offset    return yylval.int_t = AD_OFFSET;

\n+  return yylval.int_t = EOLN;
[ \t]+  /* WHITESPACE(not including EOLN), do nothing */
#[^\n]* /* COMMENT, do nothing */
[^\+\-\*\/\%\&\|\^\~\<\>\(\)\.\,\:\; \n\t]+   { yyerror("unexpected token"); exit(0); }

%%
