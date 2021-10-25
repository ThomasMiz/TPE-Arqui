#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define BIT_SCANCODE_UP 0b10000000
#define BIT_SCANCODE_DOWN 0b00000000

/* Converts a scancode value to it's ASCII representation. Returns '\0' if there's no ASCII equivalent. */
char scancodeToAscii(char scancode);

#endif