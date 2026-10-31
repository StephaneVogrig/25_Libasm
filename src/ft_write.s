section .text
    global  ft_write
    extern  __errno_location

ft_write:
    mov     rax, 1      ; sys_write
    syscall
    cmp     rax, -4095
    jb      .return
    push    rbx
    neg     rax
    mov     rbx, rax
    call    __errno_location wrt ..plt
    mov     [rax], ebx
    pop     rbx
    mov     rax, -1
.return:
    ret
