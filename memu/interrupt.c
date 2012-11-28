/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "reg.h"
#include "config.h"
#include "exception.h"
#include "interrupt.h"

#define ITQ_INIT_SIZE   0x1000

static pthread_mutex_t interrupt_mutex = PTHREAD_MUTEX_INITIALIZER;
static int interrupt_state[] = 
{
    [INTERRUPT_ENTRY_KEYBOARD_INPUT] = 0,
    [INTERRUPT_ENTRY_TIMER] = 0,
    [INTERRUPT_ENTRY_SYSCALL] = 0,
};

int interrupt_init()
{
    return MEMU_SUCCESS;
}

void interrupt_set(u32_t entry)
{
    pthread_mutex_lock(&wait_cond_mutex);
    interrupt_state[entry] = 1;
    pthread_cond_signal(&wait_cond);
    pthread_mutex_unlock(&wait_cond_mutex);

#if DUMP_INTERRUPT
    fprintf(LOG_FILE, "Interrupt: set at entry %.8X\n", entry);
#endif 
    return;
}

int interrupt_reset(u32_t entry)
{
    pthread_mutex_lock(&wait_cond_mutex);
    int ret = interrupt_state[entry];
    interrupt_state[entry] = 0;
    pthread_mutex_unlock(&wait_cond_mutex);

#if DUMP_INTERRUPT
    fprintf(LOG_FILE, "Interrupt: reset at entry %.8X\n", entry);
#endif 
    return ret;
}


int interrupt_test()
{
    u32_t exl;
    reg_cpr_read(FKREG_CPR_EXL, 0, &exl);   //EXL bit
    u32_t ie;
    reg_cpr_read(FKREG_CPR_IE, 0, &ie); //IE bit
#if DUMP_INTERRUPT && 0
    fprintf(LOG_FILE, "Interrupt: EXL=%d, IE=%d\n", exl, ie);
#endif 
    return !exl && ie;
}

u32_t interrupt_entry()
{
    static const u32_t tmp_loop[] = 
    { 
        INTERRUPT_ENTRY_SYSCALL,
        INTERRUPT_ENTRY_KEYBOARD_INPUT,
        INTERRUPT_ENTRY_TIMER,
        0,
    };
    int i;
    for (i = 0; tmp_loop[i]; i++)
        if (interrupt_state[tmp_loop[i]])
            return tmp_loop[i];
    return 0;
}

