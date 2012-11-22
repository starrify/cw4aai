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

#include "interrupt.h"
#include "reg.h"
#include "daemon.h"
#include "config.h"
#include "mem.h"
#include "memu.h"

void *membase;
size_t memsize;
unsigned int *sbase;

void daemon_init()
{
    get_dma_info(&membase, &memsize);
    sbase = membase + config.sbase_offset;
    
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
    /*
    // scroll enough space for display 
    int i;
    for (i = 0; i < config.status_display_size; i++)
        printf("\x1b%c", 'D');
    for (i = 0; i < config.status_display_size; i++)
        printf("\x1b%c", 'M');
   
    return;
    */
}

void daemon_fini()
{
    endwin();
    return;
}

void *display_daemon(void *ptr)
{
    struct timespec rqtp = { .tv_sec = 0, .tv_nsec = 10000000, };
    while (1)
    {
        nanosleep(&rqtp, NULL);
        assert(*(sbase + 8) == 0); //display mode
        unsigned int scrx = *(sbase + 9); //screen x
        unsigned int scry = *(sbase + 10); //screen y
        unsigned int fbbase = *(sbase + 12);    //fb base
        unsigned int fbhead = *(sbase + 13);    //fb head offset
        int cursor_off = (*(sbase + 14) + fbhead - fbbase) / 4;   //cursor_off
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
        move(cursor_off / scry, cursor_off % scry);

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
    while(1)
    {
        //iq for input queue
        u32_t iq_base = *(sbase + 16);
        u32_t iq_size = *(sbase + 17);
        u32_t *iq_head = (sbase + 18);
        u32_t *iq_tail = (sbase + 19);
        
        int c = getch();    //blocked input since timeout(-1);
#if DUMP_KEYBOARD
        fprintf(LOG_FILE, "Keyboard: getch()= %d\n", c);
#endif
        if ((iq_tail - iq_head + 4) % iq_size != 0) // input queue not full
        {
           *(u32_t*)(membase + iq_base + *iq_tail) = c;
            *iq_tail += 4;
 #if DUMP_KEYBOARD
            fprintf(LOG_FILE, "Keyboard: iq_tail advanced. iqhead=0x%.8X, iqtail=0x%.8X\n", 
                *iq_head, *iq_tail);
#endif
            if (*iq_tail == iq_size)
            {
#if DUMP_KEYBOARD
                fprintf(LOG_FILE, "Keyboard: iq_queue wrapped. iqhead=0x%.8X, iqtail=0x%.8X\n", 
                    *iq_head, *iq_tail);
#endif
               *iq_tail = 0;
            }
            while (1)
            {
                if (try_interrupt(INTERRUPT_ENTRY_KEYBOARD_INPUT) == MEMU_SUCCESS)
                    break;
                struct timespec rqtp = { .tv_sec = 0, .tv_nsec = 10000000, };
                nanosleep(&rqtp, NULL);
            }
 
        }
        else
        {
#if DUMP_KEYBOARD
            fprintf(LOG_FILE, "Keyboard: iq_tail full. iqhead=0x%.8X, iqtail=0x%.8X\n", 
                *iq_head, *iq_tail);
#endif
       }
    }
    return NULL;
}


