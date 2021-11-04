#include <string.h>
#include <stdint.h>

char* strcat(char* dest, const char* src) {
	char* w;
	for (w = dest; *w != '\0'; w++);
	for (; *src != '\0'; *(w++) = *(src++));
	*w = '\0';
	return dest;
}

char* strncat(char* dest, const char* src, uint64_t n) {
	char* w;
	for (w = dest; *w != '\0'; w++);
	for (; *src != '\0' && n != 0; *(w++) = *(src++), n--);
	*w = '\0';
	return dest;
}

const char* strchr(const char* str, char c) {
	for (; *str != '\0'; str++)
		if (*str == c)
			return str;
	return NULL;
}

int strcmp(const char* str1, const char* str2) {
	for (; *str1 == *str2 && *str1 != '\0'; str1++, str2++);
	return *str1 - *str2;
}

int strncmp(const char* str1, const char* str2, uint64_t n) {
	for (; *str1 == *str2 && *str1 != '\0' && n != 0; str1++, str2++, n--);
	return *str1 - *str2;
}

char* strcpy(char* dest, const char* src) {
	char* w;
	for (w = dest; *src != '\0'; *(w++) = *(src++));
	*w = '\0';
	return dest;
}

char* strncpy(char* dest, const char* src, uint64_t n) {
	char* w;
	for (w = dest; *src != '\0' && n != 0; *(w++) = *(src++), n--);
	*w = '\0';
	return dest;
}

uint64_t strlen(const char* s) {
    int l;
    for (l = 0; *s != 0; s++, l++);
    return l;
}

int itoa(int number, char* s) {
    int digits = 1;
	for (int n=number/10; n != 0; digits++, n /= 10);

    if(digits == 1) {
        s[0] = '0';
        s[1] = number + '0';
        s[2] = 0;
        return digits;
    }
	
	s[digits] = 0;    
	for (int i=digits-1; i>=0; i--) {
		s[i] = (number % 10) + '0';
		number /= 10;
	}
	
	return digits;
}