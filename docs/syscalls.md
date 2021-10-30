# Syscall documentation

## [0x00] uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
Reads up to `count` bytes from a file descriptor. If no data is available to read at the moment, blocks until there is. 

## [0x01] uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
Writes up to `count` bytes to a file descriptor.

## [0x02] uint64_t sys_time();
Returns a struct with the current system time in seconds, minutes, and hours.

## [0x07] int sys_poll(uint64_t timeout_ms);
Waits for data to become available to read using `sys_read(...)` on `STDIN`. Returns 1 if any data is available for reading, or -1 if the timeout expired.

Passing a 0 or negative timeout will make `sys_poll(...)` return immediately even if there is no available data to read. Otherwise, it only ensures at least `timeout_ms` milliseconds have passed.

# Default file descriptors
- [0] `STDIN`: Standard input
- [1] `STDOUT`: Standard output
- [2] `STDERR`: Standard error
- [3] `KBDIN` When `STDIN` brings data from the keyboard, this brings the same data but raw. `KBDIN` brings raw scancodes while `STDIN` filters them and turns them into ASCII characters.
