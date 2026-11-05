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
; if malloc failed or nmemb * size overflow,  errno is set to ENOMEM (12) by malloc
; -----------------------

section .text
    global  ft_calloc
    extern  malloc

ft_calloc:

    push     rbx

    imul     rdi, rsi
    jo      .finish
    mov     rbx, rdi
    call    malloc
    test    rax, rax
    jz      .finish
    
.loop:
    sub     rbx, 1
    mov     [rax + rbx], byte 0
    test    rbx, rbx
    jnz     .loop

.finish:
    pop     rbx
    ret
