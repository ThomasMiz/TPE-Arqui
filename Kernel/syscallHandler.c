#include <stdint.h>

uint64_t sys_read_handler(uint64_t fd, char* buf, uint64_t count) {
	return 0;
}

uint64_t sys_write_handler(uint64_t fd, const char* buf, uint64_t count) {
	return 0;
}

static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
	sys_read_handler, sys_write_handler
};

uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
	if (rax < (sizeof(syscall_handlers)/sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
		return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
	return 0;
}
