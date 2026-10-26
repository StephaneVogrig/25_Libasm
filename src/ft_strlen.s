section .text
    global  ft_strlen

ft_strlen:
    mov     rax, rdi        ; init pointeur courant
    jmp     .check
.next:
    inc     rax
.check:
    cmp     byte [rax], 0   ; fin de chaine ?
    jne     .next
    sub     rax, rdi        ; longueur = fin - debut
    ret
