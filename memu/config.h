/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stddef.h>
#include <stdio.h>
#include "memu.h"

enum _config_endianess
{
    CONFIG_ENDIAN_LITTLE    = 0,
    CONFIG_ENDIAN_BIG   = 1,
};

struct config_t
{
    size_t memsize;
    u32_t entry_offset;
    char *flash_file;
    int endianess;
    int display_refresh_rate;
    int iic_check_rate;
    char *log_filename;
    FILE *log_file;
    int status_display_size;
};

extern struct config_t config;

#endif /* CONFIG_H */
