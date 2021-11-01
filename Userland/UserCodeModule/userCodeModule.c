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

int main() {

	sys_clearscreen();

	unsigned int written = 0;

    char readbuf[32];
	
	uint64_t lastmillis = 0;
	char timebuf[32];
	
	while (1) {
		uint64_t readlen = sys_pollread(STDIN, readbuf, sizeof(readbuf)/sizeof(readbuf[0]), 1);
		uint64_t millis = sys_millis();
		if (millis != lastmillis) {
			lastmillis = millis;
			uint32_t digits = uintToBase(millis, timebuf, 10);
			sys_writeat(timebuf, digits, 0, 0, 0x7F);
		}

		if (readlen != 0) {
			unsigned int x = written % 80;
			unsigned int y = written / 80;
			sys_writeat(readbuf, readlen, x, y+1, 0x7F);
			written += readlen;
		}
	}
	
	return 69;
}
