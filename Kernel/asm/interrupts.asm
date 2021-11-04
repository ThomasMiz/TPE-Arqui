GLOBAL _cli
GLOBAL _sti
GLOBAL _hlt

GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL timerIntRoutine
GLOBAL keyboardIntRoutine
GLOBAL divideByZeroIntRoutine
GLOBAL invalidOpcodeIntRoutine
GLOBAL syscallIntRoutine

EXTERN timerIntHandler
EXTERN keyboardIntHandler
EXTERN exceptionHandler
EXTERN syscallHandler

SECTION .text

_cli:
	cli
	ret


_sti:
	sti
	ret

_hlt:
	sti
	hlt
	ret

haltcpu:
	cli
	hlt
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h, al
    pop rbp
    retn

picSlaveMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out 0A1h, al
    pop rbp
    retn

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro endHardwareInterrupt 0
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
%endmacro

%macro exceptionHandlerMaster 1
; Registers are saved in a the regdata vector in the following order:
; rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip, rflags

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

	mov rax, rsp ; We get the value of RSP when the exception ocurred by adding the amount of pushed bytes to the current value of RSP.
	add rax, 0x35
	mov [regdata+56], rax
	mov rax, [rsp] ; We get the value of RIP when the exception ocurred by taking the interruption's return address.
	mov [regdata+128], rax
	mov rax, [rsp+8] ; We get the value of RFLAGS the same way (it is pushed when an interrupt occurs).
	mov [regdata+136], rax

	mov rdi, %1
	mov rsi, regdata
	call exceptionHandler
%endmacro

timerIntRoutine:
    pushState

	call timerIntHandler

	endHardwareInterrupt
	popState
	iretq

keyboardIntRoutine:
    pushState

	; Read the scancode from the keyboard and put it in RDI.
	mov rax, 0
	in al, 60h
	mov rdi, rax
	call keyboardIntHandler

	endHardwareInterrupt
	popState
	iretq

divideByZeroIntRoutine:
    exceptionHandlerMaster 00h

invalidOpcodeIntRoutine:
    exceptionHandlerMaster 06h

syscallIntRoutine:
; syscalls params:	RDI	RSI	RDX	R10	R8	R9
; syscallHandler:	RDI RSI RDX R10 R8  RAX
; params in C are:	RDI RSI RDX RCX R8  R9
	mov rcx, r10
	mov r9, rax
	call syscallHandler
	iretq

section .bss
regdata	resq	18	; reserve space for 18 qwords (one for each register we want to show on exceptions).
