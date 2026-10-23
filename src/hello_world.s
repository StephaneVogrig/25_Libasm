section .data
    msg db `Hello World!`, 10
    msg_len equ $ - msg

section .text
    global _start

_start:
    mov rdi, 1       ; fd 1 = stdout
    mov rsi, msg
    mov rdx, msg_len
    mov rax, 1       ; sys_write
    syscall
    xor rdi, rdi
    mov rax, 60      ; sys_exit
    syscall
