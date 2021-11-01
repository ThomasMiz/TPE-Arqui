#ifndef _NAIVE_CONSOLE_H_
#define _NAIVE_CONSOLE_H_

#include <stdint.h>

void ncSetCursor(uint16_t x, uint16_t y);
void ncPrint(const char* string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

#endif