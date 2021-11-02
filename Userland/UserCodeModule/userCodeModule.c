#include <stdint.h>
#include <syscalls.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void getConsoleSize(uint32_t* width, uint32_t* height) {
	uint64_t size = sys_screensize();
	*width = (size & 0xFFFFFFFF) / 9;
	*height = (size >> 32) / 16;
}

int main() {

	uint32_t width, height;
	getConsoleSize(&width, &height);

	sys_clearscreen();

    char readbuf[32];
	
	uint64_t lastmillis = 0;
	char timebuf[32];
	uint32_t penpos = 16 * 0x10000;
	
	uint32_t tmp = uintToBase(width, timebuf, 10);
	Color color = {0x90, 0x90, 0x90};
	sys_writeat(timebuf, tmp, 60, 0, color);
	tmp = uintToBase(height, timebuf, 10);
	sys_writeat(timebuf, tmp, 70, 0, color);

	while (1) {
		uint64_t readlen = sys_pollread(STDIN, readbuf, sizeof(readbuf)/sizeof(readbuf[0]), 1);
		uint64_t millis = sys_millis();
		if (millis != lastmillis) {
			lastmillis = millis;
			uint32_t digits = uintToBase(millis, timebuf, 10);
			sys_writeat(timebuf, digits, 0, 0, color);
		}

		if (readlen != 0) {
			penpos = sys_writeat(readbuf, readlen, (penpos & 0xFFFF), (penpos >> 16), color);
			penpos = sys_writeat(readbuf, readlen, (penpos & 0xFFFF), (penpos >> 16), color);
			penpos = sys_writeat(readbuf, readlen, (penpos & 0xFFFF), (penpos >> 16), color);
			penpos = sys_writeat(readbuf, readlen, (penpos & 0xFFFF), (penpos >> 16), color);
		}
	}
	
	return 69;
}
