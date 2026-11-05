; int ft_strcmp(const char *s1, const char *s2);
;
; arguments:
; rdi = dest
; rsi = src
;
; return:
; rax = dest[i] - src[i] when they differ, or zero if src and dest are equal
; -----------------------

section .text
    global  ft_strcmp

ft_strcmp:
    xor     rcx, rcx
.loop:
    movzx   eax, byte [rdi + rcx]
    movzx   edx, byte [rsi + rcx]
    cmp     al, dl
    jne     .return
    inc     rcx
    test    al, al
    jne     .loop
.return:
    sub     eax, edx
    ret
