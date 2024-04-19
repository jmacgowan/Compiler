global _start
_start:
    mov rax, 7
    push rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
