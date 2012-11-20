/* 
 * This source file is part of MEMU: Mips EMUlator
 * by Pengyu CHEN(cpy.prefers.you@gmail.com)
 * COPYLEFT, ALL WRONGS RESERVED.
 */

#ifndef DAEMON_H
#define DAEMON_H

extern void display_init();
extern void display_fini();
extern void *display_daemon(void *ptr);
extern void *keyboard_daemon(void *ptr);

#endif /* DAEMON_H */
