/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <string.h>

#include "config.h"
#include "memu.h"
#include "hdd.h"

static void *hdd_base;
static size_t hdd_size;

int hdd_init()
{
    hdd_size = config.hdd_size;
    hdd_base = mmap(NULL, hdd_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(hdd_base != MAP_FAILED);
    
    int i;
    int hflist_sz = config.hdd_filelist_size;
    char **hflist = config.hdd_filelist;
    void *phdd = hdd_base;
    for (i = 0; i < hflist_sz; i++)
    {
        int fd = open(hflist[i], O_RDONLY | O_SYNC);
        assert(fd > 0);
        read(fd, phdd, config.hdd_sector_skip * SECTOR_SIZE);
        phdd += config.hdd_sector_skip * SECTOR_SIZE;
        close(fd);
    }

    return MEMU_SUCCESS;
}

int hdd_read(void *mem_start, size_t sector_start, size_t sector_count)
{
    memcpy(mem_start, hdd_base + sector_start * SECTOR_SIZE, sector_count * SECTOR_SIZE);
#if DUMP_HDD
    fprintf(LOG_FILE, "HDD: Read: mem_start=0x%p, sector_start=0x%.8X, sector_count=0x%.8X\n",
        mem_start, sector_start, sector_count);
#endif
    return MEMU_SUCCESS;
}

int hdd_write(void *mem_start, size_t sector_start, size_t sector_count)
{
    memcpy(hdd_base + sector_start * SECTOR_SIZE, mem_start, sector_count * SECTOR_SIZE);
#if DUMP_HDD
    fprintf(LOG_FILE, "HDD: Write: mem_start=0x%p, sector_start=0x%.8X, sector_count=0x%.8X\n",
        mem_start, sector_start, sector_count);
#endif
    return MEMU_SUCCESS;
}

