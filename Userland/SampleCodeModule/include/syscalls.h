#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3

uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
uint64_t sys_time();
int sys_poll(uint64_t timeout_ms);

#endif
