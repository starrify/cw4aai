/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <pthread.h>
#include <assert.h> 
#include <stdio.h>

#include "config.h" 
#include "decode.h"
#include "exception.h"
#include "interrupt.h"
#include "mem.h"
#include "mmu.h"
#include "reg.h"
#include "daemon.h"
//#include "iic.h"
#include "fancyterm.h"

static pthread_t display_daemon_thread;
static pthread_t keyboard_daemon_thread;
//static pthread_t iic_daemon_thread;

static void init()
{
    mem_create(config.memsize);
    mem_loadimg(config.img_file, config.img_base);
    mem_init();
    reg_init();
    daemon_init();
    
    config.log_file = fopen(config.log_filename, "w");
    assert(config.log_file);
    
    pthread_create(&display_daemon_thread, NULL, display_daemon, NULL);
    pthread_create(&keyboard_daemon_thread, NULL, keyboard_daemon, NULL);
//    pthread_create(&iic_daemon_thread, NULL, iic_daemon, NULL);
    
    return;
}

static void fini()
{
    pthread_join(display_daemon_thread, NULL);
    pthread_join(keyboard_daemon_thread, NULL);
//    pthread_join(iic_daemon_thread, NULL);
    daemon_fini();
    
    mem_destroy();
    return;
}

int main()
{
    init();
    
    reg_special_write(REG_SPECIAL_PC_ADVANCE1, config.entry_offset);
    reg_special_write(REG_SPECIAL_PC_ADVANCE2, config.entry_offset + 4);
    
    void *membase;
    size_t memsize;
    get_dma_info(&membase, &memsize);
    
    while (1)
    {
#if SINGLE_STEP
        static const int step = 500;
        static int step_count = step;
        step_count--;
        if (!step_count)
        {
            sleep(1);
            //while(getch() != 'p')
            //    ;
            step_count = step;
        }
#endif
        if (interrupt_test())
        {
            u32_t entry = interrupt_entry();
            if (entry)
            {
                 u32_t pcadv1;
                 reg_special_read(REG_SPECIAL_PC_ADVANCE1, &pcadv1);
                 reg_cpr_write(FKREG_CPR_EPC, 0, pcadv1);
                 reg_special_write(REG_SPECIAL_PC_ADVANCE1, entry);
                 reg_special_write(REG_SPECIAL_PC_ADVANCE2, entry + 4);
                       
                 reg_cpr_write(FKREG_CPR_EXL, 0, MEMU_TRUE);
#if DUMP_INTERRUPT
                 fprintf(LOG_FILE, "Interrupt: set EPC=%.8X\n", pcadv1);
#endif
            }
        }

        u32_t reg_pc = reg_advance_pc();
        u32_t code;
        u32_t paddr;
        u32_t attr;
        mmu_addr_trans(reg_pc, MEM_ACCESS_INST | MEM_ACCESS_READ, &paddr, &attr);
        mem_read(paddr, reg_pc, attr, MEM_ACCESS_LEN_4 | MEM_ACCESS_INST | MEM_ACCESS_READ, &code);
#if DUMP_FETCH
        fprintf(LOG_FILE, "Fetching: instruction %.8X at pc=%.8X\n", code, reg_pc);
#endif
        int exception = inst_execute(code);
        
        if (exception != EXCEPTION_NONE)
        {
#if DUMP_LOG
            fprintf(LOG_FILE, "Exception: %d\n", exception);
#endif
            break;
        }
        
    }
    
    fini();
    return 0;
}
