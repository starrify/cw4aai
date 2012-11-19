/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

extern const u32_t led_base;
extern const int led_size;
extern const u32_t led_offset[8];

extern void display_init();
extern void display_fini();
extern void *display_daemon(void *ptr);

#endif /* DISPLAY_H */
