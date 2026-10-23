section .data
    msg db `Hello World!`, 10, 0

section .text
    global _start

_start:

    mov rdi, msg
    mov rdx, msg

    ; loop
    mov al, [rel rdx]
    test al, al
    je sys_exit
    inc rdx
    jmp loop

    ; sys_exit
    sub rdx, rdi
    mov rdi, rdx
    mov rax, 60
    syscall
