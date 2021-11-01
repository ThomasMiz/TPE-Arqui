#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3

uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
uint64_t sys_time(void);
uint64_t sys_millis(void);
void sys_clearscreen(void);
uint64_t sys_writeat(const char* buf, uint64_t count, uint16_t x, uint16_t y, uint8_t color);
uint64_t sys_pollread(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms);

#endif
