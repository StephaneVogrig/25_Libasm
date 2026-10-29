section .text
    global  ft_strcpy

ft_strcpy:
    mov     rax, rdi                ; value = dest
    xor     ecx,ecx                 ; index =0
.loop:
    mov     dl, byte [rsi + rcx]
    mov     byte [rdi + rcx], dl
    inc     rcx                     ; index++
    test    dl, dl                  ; if dl == 0
    jnz     .loop
    ret
