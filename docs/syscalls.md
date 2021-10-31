# Syscall documentation

## [0x00] uint64_t sys_read(uint64_t fd, char* buf, uint64_t count);
Reads up to `count` bytes from a file descriptor. If no data is available to read at the moment, blocks until there is. Returns the amount of bytes read.

## [0x01] uint64_t sys_write(uint64_t fd, const char* buf, uint64_t count);
Writes up to `count` bytes to a file descriptor. Returns the amount of bytes written.

## [0x02] uint64_t sys_time();
Returns a struct with the current system time in seconds, minutes, and hours.

## [0x03] uint64_t sys_millis();
Returns the amount of milliseconds elapsed since system startup.

## [0x07] uint64_t sys_pollread(uint64_t fd, char* buf, uint64_t count, uint64_t timeout_ms);
Same as `sys_read(...)` but returns prematurely if no data becomes available after the timeout expires.

Passing a timeout of 0 will make `sys_pollread(...)` return immediately even if there is no available data to read. Otherwise, it only ensures at least `timeout_ms` milliseconds have passed.

# Default file descriptors
- [0] `STDIN`: Standard input
- [1] `STDOUT`: Standard output
- [2] `STDERR`: Standard error
- [3] `KBDIN` When `STDIN` brings data from the keyboard, this brings the same data but raw. `KBDIN` brings raw scancodes while `STDIN` filters them and turns them into ASCII characters.
