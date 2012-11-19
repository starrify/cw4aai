/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#include <assert.h>
#include <stdlib.h> 
#include "mass.tab.h"
#include "mass.yy.h"
#include "mass.h"
#include "bingen.h"
#include "symbol.h" 

#define SYMBOL_TABLE_SIZE    10007 /* shall be a not-too-big prime */
#define NULL_ADDR   -1 /* NULL_ADDR for symbol_addr, NULL won't do */

typedef struct label_ref_list_t_
{
    struct label_ref_list_t_ *next;
    unsigned int mode; 
    unsigned int lineno;
    unsigned int addr; /* address in 4 bytes relative to start of .text */
}   label_ref_list_t; /* to be used as symbol_t->misc */

typedef struct symbol_t_
{
    int type;
    const char *name;
    unsigned int addr; /* address relative in 4 bytes to start of .text */
    void *misc; /* for type-specific values, reserved */ 
}   symbol_t;

typedef struct symbol_list_t_
{
    struct symbol_list_t_ *next;
    symbol_t *symbol;
}   symbol_list_t;

static symbol_list_t *symbol_table[SYMBOL_TABLE_SIZE];

/* assuming sizeof(unsigned int) == 4 */
static inline unsigned int elfhash(const char *str)
{
    unsigned int ret = 0;
    while (*str)
    {
        ret = (ret << 4) + *str;
        unsigned int tmp = ret & 0xf0000000;
        ret ^= tmp >> 24;
        ret &= ~tmp;
        /* registers may perform faster than immediate generators */
        //ret &= 0x0fffffff; 
        str++;
    }
    return ret;
}

void symbol_table_init()
{
    /* no initialization needed */    
    return;
}

static inline symbol_list_t *symbol_list_find(symbol_list_t *symbol_list, const char *symbol_name)
{
    symbol_list_t *ret = symbol_list;
    while (ret && strcmp(ret->symbol->name, symbol_name))
        ret = ret->next;
    return ret;
}

void symbol_table_fini()
{
    /* just don't finalize it */
    return;
}

static inline symbol_list_t *symbol_list_new()
{
    return calloc(1, sizeof(symbol_list_t));
}

static inline symbol_t *symbol_new()
{
    return calloc(1, sizeof(symbol_t));
}

static inline label_ref_list_t *label_ref_list_new()
{
    return calloc(1, sizeof(label_ref_list_t));
}

/* register a label for later fill back */
void register_label(const char *label_name, int addr, int mode, int lineno)
{
    unsigned int hash = elfhash(label_name);
    symbol_list_t *symbol_list = symbol_table[hash % SYMBOL_TABLE_SIZE];
    symbol_list = symbol_list_find(symbol_list, label_name);
    symbol_t *symbol;
    
    if (!symbol_list)
    {
        symbol = symbol_new();
        symbol->type = SYMBOL_LABEL;
        symbol->name = label_name; /* lifetime of label_name is long enough */
        symbol->addr = NULL_ADDR;

        symbol_list = symbol_list_new();
        symbol_list->symbol = symbol;
        symbol_list->next = symbol_table[hash % SYMBOL_TABLE_SIZE];
        symbol_table[hash % SYMBOL_TABLE_SIZE] = symbol_list;
    }
    else
    {
        symbol = symbol_list->symbol;
        if ((symbol->addr != NULL_ADDR) && (mode & LABEL_DECLARE)) 
        {
            errmsg("%s: %d: Error: symbol `%s' is already defined\n", 
                option_input_file, lineno, symbol->name);
            exit(0);
        }
    }
    
    if ((mode & LABEL_DECLARE) && !(mode & LABEL_REFERENCE))
        symbol->addr = addr; /* symbol->addr must be NULL_ADDR before */
    else if (!(mode & LABEL_DECLARE) && (mode & LABEL_REFERENCE))
    {
        label_ref_list_t *label_ref_list = label_ref_list_new();
        label_ref_list->addr = addr;
        label_ref_list->mode = mode;
        label_ref_list->lineno = lineno;
        label_ref_list->next = symbol->misc;
        symbol->misc = label_ref_list;
    }
    else
        assert(0);

    return;
}

static inline void write_back_label_addr(symbol_t *symbol)
{
    label_ref_list_t *label_ref_list = symbol->misc;
    while (label_ref_list)
    {
        if (symbol->addr == NULL_ADDR) 
        {
            errmsg("%s: %d: Error: undeclared label '%s'\n", 
                option_input_file, label_ref_list->lineno, symbol->name);
            exit(0);
        }
        else
        {
            unsigned int target_addr;
            if (label_ref_list->mode & LABEL_ABSOLUTE)
                target_addr = symbol->addr;
            else
                target_addr = symbol->addr - label_ref_list->addr - 4;
                
            unsigned int code = buf_load_word(label_ref_list->addr);
            if (label_ref_list->mode & LABEL_16_BIT)
                code = (code & ~MASK_LOW16) | ((target_addr >> 2) & MASK_LOW16);
            else if (label_ref_list->mode & LABEL_26_BIT)
                code = (code & ~MASK_LOW26) | ((target_addr >> 2) & MASK_LOW26);
            buf_save_word(label_ref_list->addr, code);
                            
        }
        label_ref_list = label_ref_list->next;
    }
    

    return;
}

/* write back label addresses to generated binary */
void write_back_label_addr_all()
{
    int i;
    for (i = 0; i < SYMBOL_TABLE_SIZE; i++)
        if (symbol_table[i])
        {
            symbol_list_t *symbol_list = symbol_table[i];
            while (symbol_list)
            {
                if (symbol_list->symbol->type == SYMBOL_LABEL)
                    write_back_label_addr(symbol_list->symbol);
                symbol_list = symbol_list->next;               
            }
        }
    return;
}

