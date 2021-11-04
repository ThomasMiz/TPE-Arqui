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
	return rtc_getCurrentTime();
}

uint64_t sys_date_handler() {
	return rtc_getCurrentDate();
}

uint64_t sys_millis_handler() {
	return rtc_getElapsedMilliseconds();
}

void sys_clearscreen_handler() {
	scr_clear(); // TODO: change to new console when implemented
}

uint32_t sys_writeat_handler(const char* buf, uint64_t count, uint16_t x, uint16_t y, Color color) {
	scr_setPenPosition(x, y);
	scr_setPenColor(color);
	for (int i = 0; i < count; i++)
		scr_printChar(buf[i]);
	return scr_getPenX() | ((uint32_t)scr_getPenY() << 16);
}

uint64_t sys_screensize_handler() {
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
		uint64_t start_ms = rtc_getElapsedMilliseconds();
		do {
			_hlt();
			totalRead += (fd == STDIN ? kbd_readCharacters(buf + totalRead, count - totalRead) : kbd_readScancodes(buf + totalRead, count - totalRead));
		} while (totalRead == 0 && (rtc_getElapsedMilliseconds() - start_ms) < timeout_ms);
	}

	return totalRead;
}

uint64_t sys_read_handler(uint64_t fd, char* buf, uint64_t count) {
	return sys_pollread_handler(fd, buf, count, 0xFFFFFFFFFFFFFFFF);
}

void sys_drawpoint_handler(uint16_t x, uint16_t y, Color color) {
	scr_setPixel(x, y, color);
}

void sys_drawrect_handler(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color) {
	scr_drawRect(x, y, width, height, color);
}

void sys_drawline_handler(uint16_t fromX, uint16_t fromY, uint16_t toX, uint16_t toY, Color color) {
	scr_drawLine(fromX, fromY, toX, toY, color);
}

static uint64_t (*syscall_handlers[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {
	sys_read_handler, sys_write_handler, sys_time_handler, sys_millis_handler, sys_clearscreen_handler,
	sys_writeat_handler, sys_screensize_handler, sys_pollread_handler, sys_drawpoint_handler, sys_drawrect_handler,
	sys_drawline_handler, sys_date_handler
};

uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
	if (rax < (sizeof(syscall_handlers)/sizeof(syscall_handlers[0])) && syscall_handlers[rax] != 0)
		return syscall_handlers[rax](rdi, rsi, rdx, r10, r8);
	return 0;
}
