#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>
#define NULL 0

char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, uint64_t n);
const char* strchr(const char* str, char c);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, uint64_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, uint64_t n);
uint64_t strlen(const char* s);
uint64_t itoa(uint64_t number, char* s);

#endif
