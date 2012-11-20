%{
/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#include <assert.h>
#include <stdio.h>
//#include "mass.yy.h"
#include "mass.h"
#include "bingen.h"
#include "symbol.h"

static inline void warn_expand_to_multi_inst(char *file, int lineno);

%}

%union /* typedef of yylval */
{
    int int_t;
    char *str_t;
}

%token <str_t>
    /* misc */
    IDENTIFIER 
    
%token <int_t>
    /* misc */
    INTEGER L_PARENTHESIS R_PARENTHESIS COMMA COLON SEMICOLON COMMENT WHITESPACE 
    EOLN DOT DUP
    
    /* operator */
    OP_ADD OP_SUB OP_MUL OP_DIV OP_MOD OP_AND OP_OR OP_XOR OP_NOT OP_LSHIFT 
    OP_RSHIFT

    /* r-type instructions */
    INST_ADD INST_ADDU INST_AND INST_BREAK INST_DIV INST_DIVU INST_JALR
    INST_JR INST_MFHI INST_MFLO INST_MTHI INST_MTLO INST_MULT INST_MULTU 
    INST_NOR INST_OR INST_SLL INST_SLLV INST_SLT INST_SLTU INST_SRA INST_SRAV 
    INST_SRL INST_SRLV INST_SUB INST_SUBU INST_SYSCALL INST_XOR 

    /* i-type instructions */
    INST_ADDI INST_ADDIU INST_ANDI INST_BEQ INST_BGEZ INST_BGTZ INST_BLEZ 
    INST_BLTZ INST_BNE INST_LB INST_LBU INST_LH INST_LHU INST_LUI INST_LW 
    INST_LWC1 INST_ORI INST_SB INST_SLTI INST_SLTIU INST_SH INST_SW INST_SWC1 
    INST_XORI 

    /* j-type instructions */
    INST_J INST_JAL

    /* cp1 instructions */
    INST_ADD_S INST_CVT_S_W INST_CVT_W_S INST_DIV_S INST_MFC1 INST_MOV_S 
    INST_MTC1 INST_MUL_S INST_SUB_S 

    /* pseudo instructions */
    PSEUDO_INST_MOVE PSEUDO_INST_NOP PSEUDO_INST_BLT PSEUDO_INST_LI

    /* registers without dollar sign */
    REG_ZERO REG_AT REG_V0 REG_V1 REG_A0 REG_A1 REG_A2 REG_A3 REG_T0 REG_T1
    REG_T2 REG_T3 REG_T4 REG_T5 REG_T6 REG_T7 REG_S0 REG_S1 REG_S2 REG_S3 REG_S4
    REG_S5 REG_S6 REG_S7 REG_T8 REG_T9 REG_K0 REG_K1 REG_GP REG_SP REG_FP REG_RA

    /* registers with alias name $0~$31 */
    /* implemented in the lexer rules mass.lex */

%left OP_AND OP_OR OP_XOR
%left OP_LSHIFT OP_RSHIFT 
%left OP_ADD OP_SUB 
%left OP_MUL OP_DIV OP_MOD    
%nonassoc OP_NOT // also OP_ADD OP_SUB 
    
%type <str_t>
    label
    
%type <int_t>
    /* type declearations of these sub token */
    rawcode
    immediate
    expr
    label_declare
    unaryexpr
    r_type_inst_rd_rs_rt
    r_type_inst_none
    r_type_inst_rs_rt
    r_type_inst_rd_rs
    r_type_inst_rs
    r_type_inst_rd
    r_type_inst_rd_rt_sa
    r_type_inst_rd_rt_rs
    i_type_inst_rt_rs_im
    i_type_inst_rs_rt_label
    i_type_inst_rs_label
    i_type_inst_rt_im_rs
    i_type_inst_rt_im
    j_type_inst_label
    cp1_inst_fd_fs_ft
    cp1_inst_ft_fs
    cp1_inst_fd_fs
    register

%start all

%%

all: 
    statement_list ;

statement_list: 
    /* nothing */
    | statement_list statement EOLN
    ;
    
statement:
    /* empty statement */
    | instruct_statement
    | label_declare statement
    | rawcode
    ;

rawcode:
    DOT immediate { write_code($2); }
    | DUP immediate DOT immediate
    { 
        int i; 
        for (i = 0; i < $2; i++) 
            write_code($4);
    }
    ;
    
instruct_statement: 
    r_type_statement
    | i_type_statement
    | j_type_statement
    | cp1_statement
    | pseudo_statement
    ;    
    
label_declare:
    label COLON
    {  
        register_label($1, getpcount(), LABEL_DECLARE, yyget_lineno());
    }
    ;

r_type_statement: 
    r_type_statement_rd_rs_rt
    | r_type_statement_none    
    | r_type_statement_rs_rt
    | r_type_statement_rd_rs
    | r_type_statement_rs
    | r_type_statement_rd    
    | r_type_statement_rd_rt_sa
    | r_type_statement_rd_rt_rs
    ;

i_type_statement: 
    i_type_statement_rt_rs_im
    | i_type_statement_rs_rt_label
    | i_type_statement_rs_label
    | i_type_statement_rt_im_rs
    | i_type_statement_rt_im
    ;
    
j_type_statement: 
    j_type_statement_label ;
    
cp1_statement: 
    cp1_statement_fd_fs_ft
    | cp1_statement_ft_fs
    | cp1_statement_fd_fs
    ;

pseudo_statement:
    pseudo_statement_move
    | pseudo_statement_nop
    | pseudo_statement_blt
    | pseudo_statement_li
    ;
    
immediate: 
    expr
    ;
    
expr:
    L_PARENTHESIS expr R_PARENTHESIS { $$ = $2; } 
    | INTEGER   { $$ = $1; }
    | expr OP_ADD expr  { $$ = $1 + $3; }
    | expr OP_SUB expr  { $$ = $1 - $3; }
    | expr OP_MUL expr  { $$ = $1 * $3; }
    | expr OP_DIV expr  { assert($3); $$ = $1 / $3; }
    | expr OP_MOD expr  { assert($3); $$ = $1 % $3; }
    | expr OP_AND expr  { $$ = $1 & $3; }
    | expr OP_OR expr   { $$ = $1 | $3; }
    | expr OP_XOR expr  { $$ = $1 ^ $3; }
    | expr OP_LSHIFT expr   { $$ = $1 << $3; }
    | expr OP_RSHIFT expr   { $$ = $1 >> $3; }
    | unaryexpr
    ; 
    
unaryexpr:
    OP_ADD expr { $$ = +$1; }
    | OP_SUB expr   { $$ = -$2; } 
    | OP_NOT expr   { $$ = ~$2; }
    ;
    
label: 
    IDENTIFIER;

r_type_statement_rd_rs_rt: 
    r_type_inst_rd_rs_rt register COMMA register COMMA register
    {
        r_type_inst_assemble($1, $4, $6, $2, IM_NULL);
    };

r_type_statement_none:
    r_type_inst_none
    {
        r_type_inst_assemble($1, REG_NULL, REG_NULL, REG_NULL, IM_NULL);
    };
    
r_type_statement_rs_rt:
    r_type_inst_rs_rt register COMMA register
    {
        r_type_inst_assemble($1, $2, $4, REG_NULL, IM_NULL);
    };
    
r_type_statement_rd_rs:
    r_type_inst_rd_rs register COMMA register
    {
        r_type_inst_assemble($1, $4, REG_NULL, $2, IM_NULL);
    };
    
r_type_statement_rs:
    r_type_inst_rs register 
    {
        r_type_inst_assemble($1, $2, REG_NULL, REG_NULL, IM_NULL);
    };

r_type_statement_rd:
    r_type_inst_rd register
    {
        r_type_inst_assemble($1, REG_NULL, REG_NULL, $2, IM_NULL);
    };
    
r_type_statement_rd_rt_sa:
    r_type_inst_rd_rt_sa register COMMA register COMMA immediate
    {
        r_type_inst_assemble($1, REG_NULL, $4, $2, $6);
    };

r_type_statement_rd_rt_rs:
    r_type_inst_rd_rt_rs register COMMA register COMMA register
    {
        r_type_inst_assemble($1, $6, $4, $2, IM_NULL);
    };

i_type_statement_rt_rs_im: 
    i_type_inst_rt_rs_im register COMMA register COMMA immediate
    {
        i_type_inst_assemble_immediate($1, $4, $2, $6);
    };
    
i_type_statement_rs_rt_label:
    i_type_inst_rs_rt_label register COMMA register COMMA label
    {
        i_type_inst_assemble_label($1, $2, $4, $6);
    };
    
i_type_statement_rs_label: 
    i_type_inst_rs_label register COMMA label
    {
        i_type_inst_assemble_label($1, $2, REG_NULL, $4);
    };
    
i_type_statement_rt_im_rs: 
    i_type_inst_rt_im_rs register COMMA immediate L_PARENTHESIS register 
        R_PARENTHESIS
    {
        i_type_inst_assemble_immediate($1, $6, $2, $4);
    };
    
i_type_statement_rt_im:
    i_type_inst_rt_im register COMMA immediate
    {
        i_type_inst_assemble_immediate($1, REG_NULL, $2, $4);
    };
    
j_type_statement_label:
    j_type_inst_label label
    {
        j_type_inst_assemble($1, $2);
    };
    
cp1_statement_fd_fs_ft:
    cp1_inst_fd_fs_ft register COMMA register COMMA register
    {
        cp1_inst_assemble($1, $6, $4, $2);
    };
    
cp1_statement_ft_fs:
    cp1_inst_ft_fs register COMMA register
    {
        cp1_inst_assemble($1, $2, $4, REG_NULL);
    };
    
cp1_statement_fd_fs: 
    cp1_inst_fd_fs register COMMA register
    {
        cp1_inst_assemble($1, REG_NULL, $4, $2);
    };

pseudo_statement_move:
    PSEUDO_INST_MOVE register COMMA register
    {
        r_type_inst_assemble(INST_ADDU, $4, REG_NULL, $2, 0);
    }
    ;

pseudo_statement_nop:
    PSEUDO_INST_NOP 
    {
        r_type_inst_assemble(INST_ADDU, REG_NULL, REG_NULL, REG_NULL, 0);
    }
    ;

pseudo_statement_blt:
    PSEUDO_INST_BLT register COMMA register COMMA label
    {
        warn_expand_to_multi_inst(option_input_file, yyget_lineno());
        r_type_inst_assemble(INST_SLTU, $2, $4, REG_AT, 0);
        i_type_inst_assemble_label(INST_BGEZ, REG_AT, REG_NULL, $6);
    }
    ;

pseudo_statement_li:
    PSEUDO_INST_LI register COMMA immediate
    {
        if (($4 >> 16) & MASK_LOW16) /* multiple insttructions needed */
        {
            warn_expand_to_multi_inst(option_input_file, yyget_lineno());
            i_type_inst_assemble_immediate(INST_LUI, REG_NULL, $2, ($4 >> 16) & MASK_LOW16);
            i_type_inst_assemble_immediate(INST_ADDIU, $2, $2, $4 & MASK_LOW16);
        }
        else /* a single instruction would do */
            i_type_inst_assemble_immediate(INST_ADDIU, REG_ZERO, $2, $4 & MASK_LOW16);
    }
    ;

r_type_inst_rd_rs_rt: 
    INST_ADD | INST_ADDU | INST_AND | INST_NOR | INST_OR | INST_SLT | INST_SLTU 
    | INST_SUB | INST_SUBU | INST_XOR ;
    
r_type_inst_none:
    INST_BREAK | INST_SYSCALL ;
    
r_type_inst_rs_rt:
    INST_DIV | INST_DIVU | INST_MULT | INST_MULTU ;
    
r_type_inst_rd_rs:
    INST_JALR ; 

r_type_inst_rs:
    INST_JR | INST_MTHI | INST_MTLO ;

r_type_inst_rd: 
    INST_MFHI | INST_MFLO ;
    
r_type_inst_rd_rt_sa:
    INST_SLL | INST_SRA | INST_SRL ; 
       
r_type_inst_rd_rt_rs:
    INST_SLLV | INST_SRAV | INST_SRLV ;
      
i_type_inst_rt_rs_im:
    INST_ADDI | INST_ADDIU | INST_ANDI | INST_ORI | INST_SLTI | INST_SLTIU 
    | INST_XORI ;
    
i_type_inst_rs_rt_label:
    INST_BEQ | INST_BNE ;
    
i_type_inst_rs_label:
    INST_BGEZ | INST_BGTZ | INST_BLEZ | INST_BLTZ ;

i_type_inst_rt_im_rs:
    INST_LB | INST_LBU | INST_LH | INST_LHU  | INST_LW | INST_LWC1 | INST_SB 
    | INST_SH | INST_SW | INST_SWC1 ;

i_type_inst_rt_im:
    INST_LUI ;
    
j_type_inst_label:  
    INST_J | INST_JAL ;
    
cp1_inst_fd_fs_ft:
    INST_ADD_S | INST_CVT_S_W | INST_CVT_W_S | INST_DIV_S | INST_MUL_S 
    | INST_SUB_S ;
    
cp1_inst_ft_fs:
    INST_MFC1 | INST_MTC1 ;
    
cp1_inst_fd_fs: 
    INST_MOV_S ;

register: 
    REG_ZERO | REG_AT | REG_V0 | REG_V1 | REG_A0 | REG_A1 | REG_A2 | REG_A3 
    | REG_T0 | REG_T1 | REG_T2 | REG_T3 | REG_T4 | REG_T5 | REG_T6 | REG_T7 
    | REG_S0 | REG_S1 | REG_S2 | REG_S3 | REG_S4 | REG_S5 | REG_S6 | REG_S7 
    | REG_T8 | REG_T9 | REG_K0 | REG_K1 | REG_GP | REG_SP | REG_FP | REG_RA ;
    
%%

static inline void warn_expand_to_multi_inst(char *file, int lineno)
{
    if (option_warning)
        errmsg(
            "%s: %d: Warning: Macro instruction expanded into multiple instructions",
            file, lineno
            );
    return;
}

