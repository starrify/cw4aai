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

typedef struct _hddfile_t
{
    char *filename;
    int secstart;
    int secsize;
}   hddfile_t;

struct config_t
{
    size_t memsize;
    u32_t entry_offset;
    u32_t sbase_offset;

    int endianess;
    int display_refresh_interval;
    int timer_interval;

    char *log_filename;
    FILE *log_file;

    size_t hdd_size;
    int hdd_filecnt;
    hddfile_t *hdd_filelist;
};

extern struct config_t config;
extern int cfg_init();
extern int cfg_fini();

#endif /* CONFIG_H */
