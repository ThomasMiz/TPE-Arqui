#ifndef _LIBASM_H_
#define _LIBASM_H_

#include <stdint.h>

void runInvalidOpcode(void);

/* Gets the value of the following registers, in the following order:
rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15 */
const uint64_t* dumpRegisters(void);

#endif
