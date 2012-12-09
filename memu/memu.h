/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef MEMU_H
#define MEMU_H

#include <pthread.h>

#ifndef NULL
# define NULL   (void*)0
#endif

#define RELEASE "dev"
#define VERSION "0.2.1"
#define NAME    "MEMU: Mips EMUlator"
#define AUTHOR  "Pengyu CHEN(cpy.prefers.you@gmail.com)"

#define MASK(n, a, b)   ((n) & ((u32_t)(-1) >> (31 - (a))) & (- (1 << (b))))
#define MASKSHR(n, a, b)    ((u32_t)MASK((n), (a), (b)) >> (b))
#define MASKSHRSIGNEXT(n, a, b) ((i32_t)((u32_t)MASK((n), (a), (b)) << (31 - (a))) >> (31 - (a) + (b)))

#define MASK64(n, a, b) ((n) & ((u64_t)(-1ll) >> (63 - (a))) & (- (1llu << (b))))
#define MASK64SHR(n, a, b)  ((u64_t)MASK64((n), (a), (b)) >> (b))

#define SINGLE_STEP 0

#define DUMP_INST   1
#define DUMP_MEM    1
#define DUMP_MMU    0
#define DUMP_HDD    1
#define DUMP_DMA    1
#define DUMP_REG_CPR    0
#define DUMP_REG_SHADOW    0
#define DUMP_FETCH  1
#define DUMP_DISPLAY    1
#define DUMP_KEYBOARD   1
#define DUMP_SPEAKER    1
#define DUMP_INTERRUPT  1
#define DUMP_DECODE 0
#define DUMP_IIC    0
#define CONFIG_FILE "config.ini"
#define LOG_FILE    config.log_file

typedef unsigned long long  u64_t;
typedef unsigned int   u32_t;
typedef long long i64_t;
typedef int i32_t;
typedef short   i16_t;
typedef char    i8_t;

enum _memu_status 
{
    MEMU_SUCCESS,
    MEMU_FAILURE,
};

enum _memu_bool
{
    MEMU_FALSE,
    MEMU_TRUE,
};

extern int wait;
extern pthread_mutex_t wait_cond_mutex;
extern pthread_cond_t wait_cond;

#endif // MEMU_H
