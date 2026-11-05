; size_t strlen(const char *s);
;
; arguments:
; rdi = s
;
; return:
; rax = length of s, '\0' excluded
; -----------------------

section .text
    global  ft_strlen

ft_strlen:
    xor     eax, eax            ; len = 0
    jmp     .check
.loop:
    inc     rax                 ; len++
.check:
    cmp     byte [rdi + rax], 0
    jne     .loop
    ret
