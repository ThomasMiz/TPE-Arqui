#include <stdint.h>
#include <syscalls.h>

int main() {

	char buf[32];
	while (1) {
		uint64_t len = sys_pollread(STDIN, buf, sizeof(buf)/sizeof(buf[0]), 3000);
		if (len == 0)
			sys_write(STDOUT, "timeout!", 8);
		else
			sys_write(STDOUT, buf, len);
	}
	
	return 69;
}
