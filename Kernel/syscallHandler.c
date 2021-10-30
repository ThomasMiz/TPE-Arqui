#include <stdint.h>
#include <defs.h>
#include <time.h>
#include <interrupts.h>
#include <naiveConsole.h>

uint64_t sys_read_handler(uint64_t fd, char* buf, uint64_t count) {
	// Any file descriptor that isnt STDIN or KBDIN gets ignored
	if (fd != STDIN && fd != KBDIN)
		return 0;
	
	// We do an initial read of the available characters
	unsigned int totalRead = (fd == STDIN ? kbd_readCharacters(buf, count) : kbd_readScancodes(buf, count));

	// If no characters were available, we block until some are.
	while (totalRead == 0) {
		_hlt();
		totalRead += (fd == STDIN ? kbd_readCharacters(buf + totalRead, count - totalRead) : kbd_readScancodes(buf + totalRead, count - totalRead));
	}

	return totalRead;
}

uint64_t sys_write_handler(uint64_t fd, const char* buf, uint64_t count) {
	for (int i = 0; i < count; i++)
		ncPrintChar(buf[i]); // only for testing!!
	return count;
}

uint64_t sys_time_handler() {
	return getActualTime();
}

uint64_t sys_poll_handler(uint64_t timeout_ms) {
	return -1; // TODO
}

static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
	sys_read_handler, sys_write_handler, sys_time_handler, 0, 0, 0, 0, sys_poll_handler
};

uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
	if (rax < (sizeof(syscall_handlers)/sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
		return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
	return 0;
}
