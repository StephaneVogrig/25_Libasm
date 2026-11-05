; char *strdup(const char *s);
;
; arguments:
; rdi = s
;
; return:
; rax = NULL or ptr to copy of s allowed by malloc
;
; Note :
; if malloc failed errno is set to ENOMEM (12) by malloc
; -----------------------

section .text
    global  ft_strdup
    extern  ft_strlen
    extern  ft_strcpy
    extern  malloc

ft_strdup:
    push    rbp
    mov     rbp, rdi                    ; rbp = *s

    call    ft_strlen
    inc     rax                         ; len++ to add '\0'

    mov     rdi, rax
    call    malloc wrt ..plt            ; rax = *dup
    test    rax, rax
    jz      .finish

    mov     rdi, rax
    mov     rsi, rbp
    call    ft_strcpy                   ; rax = ptr to copy

.finish:
    pop     rbp
    ret
