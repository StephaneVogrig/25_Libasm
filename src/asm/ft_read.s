section .text
    global  ft_read
    extern  __errno_location

ft_read:
    push    rbx
    mov     rax, 0                      ; sys_read = 0
    syscall
    cmp     rax, -4095                  ; -errno = -1 .. -4095 (kernel convention)
    jb      .return
    neg     rax                         ; errno is positive
    mov     rbx, rax
    call    __errno_location wrt ..plt  ; get the errno address
    mov     [rax], ebx                  ; save the errno value
    mov     rax, -1                     ; return -1
.return:
    pop     rbx
    ret
