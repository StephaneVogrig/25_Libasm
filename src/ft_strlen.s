section .text
    global ft_strlen

ft_strlen:
    mov rax, rdi
.next:
    mov cl, [rax]
    test cl, cl
    je .done
    inc rax
    jmp .next
.done:
    sub rax, rdi
    ret
