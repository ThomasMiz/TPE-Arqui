#ifndef _COREUTIL_H_
#define _COREUTIL_H_

#include <stdint.h>

#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

char scancodeToAscii(char scancode);

#endif