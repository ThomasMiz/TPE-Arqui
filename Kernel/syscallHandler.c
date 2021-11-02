#include <stdint.h>
#include <defs.h>
#include <time.h>
#include <interrupts.h>
#include <keyboard.h>
#include <video.h>

uint64_t sys_write_handler(uint64_t fd, const char* buf, uint64_t count) {
	for (int i = 0; i < count; i++)
		scr_printChar(buf[i]); // only for testing!! TODO: make this nice & check filedescriptor
	return count;
}

uint64_t sys_time_handler() {
	return getActualTime();
}

uint64_t sys_millis_handler() {
	return getElapsedMilliseconds();
}

void sys_clearscreen() {
	scr_clear(); // TODO: change to new console when implemented
}

uint32_t sys_writeat(const char* buf, uint64_t count, uint16_t x, uint16_t y, Color color) {
	scr_setPenPosition(x, y);
	scr_setPenColor(color);
	for (int i = 0; i < count; i++)
		scr_printChar(buf[i]);
	return scr_getPenX() | ((uint32_t)scr_getPenY() << 16);
}

uint64_t sys_screensize() {
	return scr_getWidth() | ((uint64_t)scr_getHeight() << 32);
}

uint64_t sys_pollread_handler(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms) {
	// Any file descriptor that isnt STDIN or KBDIN gets ignored
	if (fd != STDIN && fd != KBDIN)
		return 0;
	
	// We do an initial read of the available characters
	unsigned int totalRead = (fd == STDIN ? kbd_readCharacters(buf, count) : kbd_readScancodes(buf, count));

	if (timeout_ms != 0) {
		// We block until data was read or the timeout expires
		uint64_t start_ms = getElapsedMilliseconds();
		do {
			_hlt();
			totalRead += (fd == STDIN ? kbd_readCharacters(buf + totalRead, count - totalRead) : kbd_readScancodes(buf + totalRead, count - totalRead));
		} while (totalRead == 0 && (getElapsedMilliseconds() - start_ms) < timeout_ms);
	}

	return totalRead;
}

uint64_t sys_read_handler(uint64_t fd, char* buf, uint64_t count) {
	return sys_pollread_handler(fd, buf, count, 0xFFFFFFFFFFFFFFFF);
}

static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
	sys_read_handler, sys_write_handler, sys_time_handler, sys_millis_handler, sys_clearscreen, sys_writeat, sys_screensize, sys_pollread_handler
};

uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
	if (rax < (sizeof(syscall_handlers)/sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
		return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
	return 0;
}
