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
	pushState

	mov rdi, %1
	call exceptionHandler

	popState
	iretq
%endmacro

timerIntRoutine:
    pushState

	call timerIntHandler

	endHardwareInterrupt
	popState
	iretq

keyboardIntRoutine:
    pushState

	; Read the scancode from the keyboard and put it in RDI
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
