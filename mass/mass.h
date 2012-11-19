/* 
 * This source file is part of MASS: Mips ASSembler 
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#ifndef MASS_H
#define MASS_H

#define RELEASE "dev"
#define VERSION "0.2.1"
#define NAME    "MASS: Mips ASSembler"
#define AUTHOR  "Pengyu CHEN(cpy.prefers.you@gmail.com)"

#define IM_NULL 0x0
#define REG_NULL REG_ZERO

enum OPTIONS_
{
    BAD_OPTION,
    BYTEORDER_BE,
    BYTEORDER_LE,
    OPTION_COUNT
};

extern void errmsg(char *msg, ...);
extern int option_byteorder;
extern int option_output_binary;
extern int option_warning;
extern char *option_input_file;

#endif // MASS_H
