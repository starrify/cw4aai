/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */
 
#include <stddef.h>
#include "config.h"

struct config_t config = 
{
    .memsize    = 0x20000000, /* 512MB */
    .entry_offset   = 0x00400000,
    .img_file   = "sys.img",
    .img_base   = 0x00400000,
    .sbase_offset   = 0x00008000,
    .flash_file = "yamon-02.21.fl",
    .endianess  = CONFIG_ENDIAN_LITTLE,
    .display_refresh_rate   = 20000000,
    .iic_check_rate = 100000,
    //.log_filename   = "/tmp/1.log",
    .log_filename = "/dev/null",
    .log_file   = NULL, /* to be initialized */
    .status_display_size    = 19,
    .hdd_size   = 0x004000000l, // 256MB
    .hdd_sector_skip    = 0x1000, // 2MB for each file
    .hdd_filelist_size  = 3,
    .hdd_filelist = { "sys.img", "app1.img", "app2.img" },
};
