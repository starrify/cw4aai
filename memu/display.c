/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <ncurses.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "reg.h"
#include "display.h"
#include "config.h"
#include "mem.h"

const u32_t led_base    = 0x1F000400;
const int led_size  = 8;
const u32_t led_offset[]    = { 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50 };
   
void *membase;
void *memsize;
unsigned long *sbase;

void display_init()
{
    get_dma_info(&membase, &memsize);
    sbase = membase + 0x00004000;

    initscr();
    printw("Hello, world!");
    refresh();
    getch();
    printw("111111111111Hello, world!");
    refresh();
    getch();
    endwin();
    return;
    /* scroll enough space for display */
    int i;
    for (i = 0; i < config.status_display_size; i++)
        printf("\x1b%c", 'D');
    for (i = 0; i < config.status_display_size; i++)
        printf("\x1b%c", 'M');
   
    return;
}

void display_fini()
{
    return;
}

void *display_daemon(void *ptr)
{
    assert(*(sbase + 32) == 0); //display mode
    unsigned int scr_x = *(sbase + 33); //screen x
    unsigned int scr_y = *(sbase + 34); //screen y
    unsigned int fbbase = *(sbase + 36);    //fb base
    unsigned int fbhead = *(sbase + 37);    //fb head offset

    initscr();
    return;

    while (1)
    {
        struct timespec rqtp = { .tv_sec = 0, .tv_nsec = config.display_refresh_rate };
        nanosleep(&rqtp, NULL);
        printf("\x1b%c", '7');  /* saves cursor position */

        printf("MEMU(Mips EMUlator) of a MIPS Malta board with MIPS32 4K LV core.\n");
        printf("\n");
        printf("On board LED: ");
        int i;
        for (i = 0; i < led_size; i++)
            printf("%c", *(char*)(membase + led_base + led_offset[i]));
        printf("\n\n");
        printf("Values of general purpose registers:\n");
        for (i = 0; i < NUMBER_OF_REG_GPRS; i++)
        {
            if (!(i & 3))
                printf("%.4X: ", i);
            i32_t regdata;
            reg_gpr_read(i, &regdata);
            printf("0x%.8X%s", regdata, ((i + 1) & 3) ? " " : "\n");
        }       
        printf("\n");
        printf("Values of special registers:\n");
        const int reg_special_id[] = { REG_SPECIAL_PC, REG_SPECIAL_HI, REG_SPECIAL_LO, };
        const char *reg_special_name[] = { "PC", "HI", "LO", };
        for (i = 0; i < sizeof(reg_special_id) / sizeof(reg_special_id[0]); i++)
        {
            i32_t regdata;
            reg_special_read(reg_special_id[i], &regdata);
            printf("REG_SPECIAL_%s: 0x%.8X\n", reg_special_name[i], regdata);
        }

        printf("\x1b%c", '8');  /* restores cursor position */   
    }
    return NULL;
}
