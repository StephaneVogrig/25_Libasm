section .text
    global  ft_strcmp

ft_strcmp:
    xor     rcx, rcx
.loop:
    movzx   eax, byte [rdi + rcx]
    movzx   edx, byte [rsi + rcx]
    cmp     al, dl
    jne      .return
    inc     rcx
    test    al, al
    jne     .loop
.return:
    sub     eax, edx
    ret
