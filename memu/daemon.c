/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <ncurses.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "reg.h"
#include "daemon.h"
#include "config.h"
#include "mem.h"
#include "memu.h"

void *membase;
size_t memsize;
unsigned int *sbase;

void display_init()
{
    get_dma_info(&membase, &memsize);
    sbase = membase + 0x00008000;
    
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    timeout(-1);

    start_color();
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(0, COLOR_WHITE, COLOR_BLACK);

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
    endwin();
    return;
}

void *display_daemon(void *ptr)
{
    while (1)
    {
        assert(*(sbase + 8) == 0); //display mode
        unsigned int scrx = *(sbase + 9); //screen x
        unsigned int scry = *(sbase + 10); //screen y
        unsigned int fbbase = *(sbase + 12);    //fb base
        unsigned int fbhead = *(sbase + 13);    //fb head offset
        int cursor_off = *(sbase + 14);   //cursor_off
        int chrsz = 4;
        int scrsize = scrx * scry * chrsz;
        memcpy(membase + fbbase + scrsize, membase + fbbase, fbhead * chrsz);

        int i;
        for (i = 0; i < scrx; i++)
        {
            int j;
            for (j = 0; j < scry; j++)
            {
                unsigned int p = fbbase + fbhead + (i * scry + j) * chrsz;
                unsigned int c = *(unsigned int*)(p + membase);
                mvaddch(i, j, c);
#if DUMP_DISPLAY
                fprintf(LOG_FILE, "DISPLAY: %.8X: %d at %d %d\n", p, c, i, j);
#endif
            }
        }
        move((cursor_off / 4) / scry, (cursor_off / 4) % scry);

        refresh();
    }

    return;
    /*
    while (1)
    {
        struct timespec rqtp = { .tv_sec = 0, .tv_nsec = config.display_refresh_rate };
        nanosleep(&rqtp, NULL);
        printf("\x1b%c", '7');  // saves cursor position 

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

        printf("\x1b%c", '8');  // restores cursor position 
    }
    */
    return NULL;
}

void *keyboard_daemon(void *ptr)
{
    unsigned int fbbase = *(sbase + 12); 
    while(1)
    {
        int c = getch();
        u32_t *p = membase + fbbase;
        if (c != ERR)
            *p = c;
    }
    return NULL;
}


