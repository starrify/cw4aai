/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include "reg.h"
#include "config.h"
#include "exception.h"
#include "interrupt.h"

int interrupt_init()
{
    return MEMU_SUCCESS;
}


int try_interrupt(u32_t entry)
{
    u32_t exl;
    reg_cpr_read(FKREG_CPR_EXL, 0, &exl);   //EXL bit
    u32_t ie;
    reg_cpr_read(FKREG_CPR_IE, 0, &ie); //IE bit
#if DUMP_INTERRUPT
    fprintf(LOG_FILE, "Interrupt: try to interrput at entry %.8X\n", entry);
#endif 
    if (exl || !ie)
    {
#if DUMP_INTERRUPT
        fprintf(LOG_FILE, "Interrupt: failed. EXL=%d, IE=%d\n", exl, ie);
#endif 
        return MEMU_FAILURE;
    }
    
    u32_t pcadv1;
    reg_special_read(REG_SPECIAL_PC_ADVANCE1, &pcadv1);
    reg_cpr_write(FKREG_CPR_EPC, 0, pcadv1);
    reg_special_write(REG_SPECIAL_PC_ADVANCE1, entry);
    reg_special_write(REG_SPECIAL_PC_ADVANCE2, entry + 4);  

    reg_cpr_write(FKREG_CPR_EXL, 0, MEMU_TRUE);
#if DUMP_INTERRUPT
    fprintf(LOG_FILE, "Interrupt: set EPC=%.8X\n", pcadv1);
#endif 
    return MEMU_SUCCESS;

}
