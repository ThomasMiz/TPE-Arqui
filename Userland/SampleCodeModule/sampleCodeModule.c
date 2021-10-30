#include <stdint.h>
#include <syscalls.h>

int main() {
	
	char buf[32];
	while(1) {
		uint64_t len = sys_read(STDIN, buf, sizeof(buf)/sizeof(buf[0]));
		sys_write(STDOUT, buf, len);
	}
	
	return 69;
}
