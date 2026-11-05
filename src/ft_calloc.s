; void *ft_calloc(size_t nmemb, size_t size);
;
; arguments:
; rdi = nmemb
; rsi = size
;
; return:
; rax = NULL or ptr allowed by malloc
;
; Note :
; if nmemb * size overflow, errno is set to ENOMEM (12), return NULL (0)
; if malloc failed errno is set by malloc, return NULL (0)
; -----------------------

%define ENOMEM 12

section .text
    global  ft_calloc
    extern  malloc
    extern  __errno_location

ft_calloc:
    push    rbx

    mov     rax, rdi
    mul     rsi                         ; RDX:RAX = nmemb * size
    jo      .overflow                   ; if overflow

    mov     rbx, rax
    mov     rdi, rax
    call    malloc wrt ..plt
    test    rax, rax
    jz      .finish                     ; if malloc = NULL
    test    rbx, rbx
    jz      .finish

.loop:
    sub     rbx, 1
    mov     [rax + rbx], byte 0
    test    rbx, rbx
    jnz     .loop

.finish:
    pop     rbx
    ret

.overflow:
    call    __errno_location wrt ..plt  ; get the errno address
    mov     [rax], dword ENOMEM               ; save the errno value
    xor     eax, eax                    ; return NULL (0)
    jmp     .finish
