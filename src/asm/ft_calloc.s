; void *ft_calloc(size_t nmemb, size_t size);
;
; arguments:
; rdi = nmemb
; rsi = size
;
; return:
; rax = NULL or ptr allowed by malloc
;
; Note:
; if nmemb * size overflow, errno is set to ENOMEM, return NULL (0)
; on error, errno is set by malloc, return NULL (0)
; -----------------------

%define ENOMEM 12

section .text
    global  ft_calloc
    extern  malloc
    extern  __errno_location

ft_calloc:
    push    rbx                         ; save rbx + align stack to 16 bytes

    mov     rax, rdi                    ; rax = nmemb
    mul     rsi                         ; rdx:rax = nmemb * size
    jo      .overflow                   ; OF set if rdx != 0 (nmemb * size overflows 64 bits)

    mov     rbx, rax                    ; keep malloc size across the call
    mov     rdi, rax
    call    malloc wrt ..plt
    test    rax, rax
    jz      .finish                     ; malloc failed, return NULL
    test    rbx, rbx
    jz      .finish                     ; size == 0

.loop:
    sub     rbx, 1                      ; rbx-- (ZF = 1 when rbx == 0)
    mov     byte [rax + rbx], 0
    jnz     .loop

.finish:
    pop     rbx
    ret

.overflow:
    call    __errno_location wrt ..plt  ; get the errno address
    mov     [rax], dword ENOMEM         ; save the errno value
    xor     eax, eax                    ; return NULL (0)
    jmp     .finish
