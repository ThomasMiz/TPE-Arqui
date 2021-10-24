#include <string.h>

void* memset(void* destination, uint8_t c, uint64_t length) {
	while(length--)
		((char*)destination)[length] = c;
	return destination;
}

void* memcpy(void* destination, const void* source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0) {
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	} else {
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

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