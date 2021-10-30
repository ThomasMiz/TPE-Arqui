GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_time
GLOBAL sys_poll

section .text
sys_read:
    mov rax, 0x00
    int 80h
    ret
    
sys_write:
    mov rax, 0x01
    int 80h
    ret
    
sys_time:
    mov rax, 0x02
    int 80h
    ret
    
sys_poll:
    mov rax, 0x07
    int 80h
    ret
