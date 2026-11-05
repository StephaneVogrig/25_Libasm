; char *ft_strdup(const char *s);
;
; arguments:
; rdi = s
;
; return:
; rax = pointer to a malloc'd copy of s, or NULL on failure
;
; Note:
; on error, errno is set to ENOMEM by malloc
; -----------------------

section .text
    global  ft_strdup
    extern  ft_strlen
    extern  ft_strcpy
    extern  malloc

ft_strdup:
    push    rbp                         ; save rbp + align stack to 16 bytes
    mov     rbp, rdi                    ; keep s across the call

    call    ft_strlen
    inc     rax                         ; len++ to add '\0'

    mov     rdi, rax
    call    malloc wrt ..plt            ; rax = dup or NULL
    test    rax, rax
    jz      .finish

    mov     rdi, rax
    mov     rsi, rbp
    call    ft_strcpy                   ; rax = ptr to copy

.finish:
    pop     rbp
    ret
