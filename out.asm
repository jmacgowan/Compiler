global _start
_start:
    mov rax, 3
    push rax
    mov rax, 3
    push rax
    mov rax, 3
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    mov rax, 3
    push rax
    push QWORD [rsp + 8]

    push QWORD [rsp + 8]

    pop rax
    pop rbx
    imul rax, rbx
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
