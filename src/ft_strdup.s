; char *strdup(const char *s);
;
; arguments:
; rsi = s
;
; return:
; rax = ptr to copy of s allowed by malloc or NULL
;
; Note :
; if malloc failed errno is set to ENOMEM (12)
; -----------------------

section .text
    global  ft_strdup
    extern  ft_strlen
    extern  ft_strcpy
    extern  malloc
    extern __errno_location

ft_strdup:
    mov     rbp, rsp
    push    rbp
    push    rbx
    sub     rsp, 8

    mov     rbp, rdi                    ; rbp = *s
    call    ft_strlen
    inc     rax                         ; len++ to add '\0'
    mov     rbx, rax                    ; rbx = size of s including '\0'

    mov     rdi, rax
    call    malloc wrt ..plt            ; rax = *dup
    test    rax, rax
    jz      .malloc_error

    mov     rdi, rax
    mov     rsi, rbp
    mov     rdx, rbx
    call    ft_strcpy                   ; rax = ptr to copy
    jmp     .finish
.malloc_error:
    call    __errno_location wrt ..plt  ; get the errno address
    mov     [rax], ebx                  ; save the errno value
    xor     rax, rax
.finish:
    add     rsp, 8
    pop     rbx
    pop     rbp
    ret
