; ssize_t ft_read(int fd, void *buf, size_t count);
;
; arguments:
; rdi = fd
; rsi = buf
; rdx = count
;
; return:
; rax = number of bytes read, or -1 on error
;
; Note:
; on error, errno is set to the positive error code returned by the kernel
; -----------------------

%define SYS_READ 0

section .text
    global  ft_read
    extern  __errno_location

ft_read:
    push    rbx                         ; save rbx + align stack to 16 bytes
    mov     rax, SYS_READ
    syscall
    cmp     rax, -4095                  ; rax >= -4095 (unsigned) => kernel error
    jb      .return                     ; below => success, return as is
    neg     rax                         ; errno is positive
    mov     rbx, rax                    ; keep errno across the call
    call    __errno_location wrt ..plt  ; get the errno address
    mov     [rax], ebx                  ; save the errno value
    mov     rax, -1                     ; return -1
.return:
    pop     rbx
    ret
