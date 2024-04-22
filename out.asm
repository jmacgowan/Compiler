global _start
_start:
    mov rax, 7
    push rax
    mov rax, 6
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    mov rax, 5
    push rax
    mov rax, 4
    push rax
    push QWORD [rsp + 16]

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
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
