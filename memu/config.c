/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <assert.h>
#include <iniparser.h>
#include <stddef.h>
#include "config.h"
#include "memu.h"

struct config_t config = 
{
    .memsize    = 0x20000000, /* 512MB */
    .entry_offset   = 0x00400000,
    .sbase_offset   = 0x00008000,

    .endianess  = CONFIG_ENDIAN_LITTLE,
    .display_refresh_interval   = 20000000,
    .timer_interval = 10000000,

    .log_filename   = "/tmp/memu0.log",
    //.log_filename = "/dev/null",
    .log_file   = NULL, /* to be initialized */

    .hdd_size   = 0x004000000l, // 256MB
    .hdd_filecnt    = 0,
    .hdd_filelist   = NULL,
};

static dictionary *dict;

int cfg_init()
{
    dict = iniparser_load(CONFIG_FILE);

    config.memsize = iniparser_getint(dict, "mem:memsize", config.memsize);
    config.entry_offset = iniparser_getint(dict, "mem:entry_offset", config.entry_offset);
    config.sbase_offset = iniparser_getint(dict, "mem:sbase_offset", config.sbase_offset);

    config.endianess = iniparser_getint(dict, "misc:endianess", config.endianess);
    config.display_refresh_interval = iniparser_getint(dict, "misc:display_refresh_interval", config.display_refresh_interval);
    config.timer_interval = iniparser_getint(dict, "misc:timer_interval", config.timer_interval);
    config.log_filename = strdup(iniparser_getstring(dict, "misc:log_file", config.log_filename));

    config.hdd_size = iniparser_getint(dict, "hdd:hdd_size", config.hdd_size);
    config.hdd_filecnt = iniparser_getint(dict, "hdd:hdd_filecnt", config.hdd_filecnt);
    config.hdd_filelist = malloc(config.hdd_filecnt * sizeof(hddfile_t));
    int i;
    for (i = 0; i < config.hdd_filecnt; i++)
    {
        char *s;
        asprintf(&s, "hdd:filename_%d", i);
        config.hdd_filelist[i].filename = strdup(iniparser_getstring(dict, s, NULL));
        free(s);
        asprintf(&s, "hdd:secstart_%d", i);
        config.hdd_filelist[i].secstart = iniparser_getint(dict, s, 0);
        free(s);
        asprintf(&s, "hdd:secsize_%d", i);
        config.hdd_filelist[i].secsize = iniparser_getint(dict, s, 0);
        free(s);
    }

    iniparser_freedict(dict);

    config.log_file = fopen(config.log_filename, "w");
    assert(config.log_file);


    return MEMU_SUCCESS;
}

int cfg_fini()
{
    fclose(config.log_file);
    return MEMU_SUCCESS;
}
