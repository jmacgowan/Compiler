global _start
_start:
    mov rax, 8
    push rax
    mov rax, 0
    push rax
    mov rax, 6
    push rax
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
