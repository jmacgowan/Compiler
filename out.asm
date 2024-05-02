global _start
_start:
    mov rax, 6
    push rax
    mov rax, 0
    push rax
    mov rax, 6
    push rax
    push QWORD [rsp + 8]

    pop rax
    mov[rsp + 0], rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
