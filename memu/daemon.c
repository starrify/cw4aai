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

static void *membase;
static size_t memsize;
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
}

void daemon_fini()
{
    endwin();
    return;
}

void *display_daemon(void *ptr)
{
    struct timespec rqtp = { .tv_sec = 0, .tv_nsec = config.display_refresh_interval, };
    while (1)
    {
        nanosleep(&rqtp, NULL);
        assert(*(sbase + 8) == 0); //display mode
        unsigned int scrx = *(sbase + 9); //screen x
        unsigned int scry = *(sbase + 10); //screen y
        unsigned int fbbase = *(sbase + 12);    //fb base
        unsigned int fbhead = *(sbase + 13);    //fb head offset
        int cursor_off = (*(i32_t*)(sbase + 14) - (i32_t)fbhead) / 4;   //cursor_off
        if (cursor_off < 0)
            cursor_off += scrx * scry;
        int chrsz = 4;
        int scrsize = scrx * scry * chrsz;
        memcpy(membase + fbbase + scrsize, membase + fbbase, fbhead * chrsz);
#if DUMP_DISPLAY
        fprintf(LOG_FILE, "Display: memcpy: %.8X -> %.8X, size: %X\n", fbbase, fbbase + scrsize, fbhead);
#endif

        int i;
        for (i = 0; i < scrx; i++)
        {
            int j;
            for (j = 0; j < scry; j++)
            {
                unsigned int p = fbbase + fbhead + (i * scry + j) * chrsz;
                unsigned int c = *(unsigned int*)(p + membase);
                mvaddch(i, j, c);
//#if DUMP_DISPLAY
//                fprintf(LOG_FILE, "DISPLAY: %.8X: %d at %d %d\n", p, c, i, j);
//#endif
            }
        }
        assert(scry);
        move(cursor_off / scry, cursor_off % scry);

        refresh();
    }

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
            interrupt_set(INTERRUPT_ENTRY_KEYBOARD_INPUT);
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

void *timer_daemon(void *ptr)
{
    struct timespec rqtp = { .tv_sec = 0, .tv_nsec = config.timer_interval, };
    while (1)
    {
        nanosleep(&rqtp, NULL);
        interrupt_set(INTERRUPT_ENTRY_TIMER);
    }
    return NULL;
}
