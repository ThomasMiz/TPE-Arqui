#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define KBDIN 3

/* The width of a character in pixels. */
#define CHAR_WIDTH 9
/* The height of a character in pixels. */
#define CHAR_HEIGHT 16

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
} Color;

uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
uint64_t sys_time(void);
uint64_t sys_millis(void);
void sys_clearscreen(void);
uint32_t sys_writeat(const char* buf, uint64_t count, uint16_t x, uint16_t y, Color color);
uint64_t sys_screensize();
uint64_t sys_pollread(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms);

#endif