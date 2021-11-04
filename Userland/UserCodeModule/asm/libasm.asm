GLOBAL dumpRegisters
GLOBAL runInvalidOpcode

section .text

runInvalidOpcode:
	db	0xFF
	ret ; Necessary? Not really... Just here for good measure.

dumpRegisters:
; rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15
	mov [regdata], rax
	mov [regdata+8], rbx
	mov [regdata+16], rcx
	mov [regdata+24], rdx
	mov [regdata+32], rsi
	mov [regdata+40], rdi
	mov [regdata+48], rbp
	mov [regdata+64], r8
	mov [regdata+72], r9
	mov [regdata+80], r10
	mov [regdata+88], r11
	mov [regdata+96], r12
	mov [regdata+104], r13
	mov [regdata+112], r14
	mov [regdata+120], r15

	mov rax, rsp ; We get the value of RSP when dumpRegisters was called by adding the amount of pushed bytes to the current value of RSP.
	add rax, 0x08
	mov [regdata+56], rax

    mov rax, regdata ; The return value of this function is a pointer to the vector with the register data.
    ret

section .bss
regdata	resq	16	; reserve space for 17 qwords (one for each register we want to show inforeg).
