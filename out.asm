global _start
_start:
    mov rax, 6
    push rax
    mov rax, 3
    push rax
    mov rax, 4
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jl condition_true_1
    jmp condition_end_1
condition_true_1:
    mov rax, 5
    push rax
    mov rax, 60
    pop rdi
    syscall
    add rsp,0
condition_end_1:
    mov rax, 0
    push rax
    mov rax, 5
    push rax
    add rsp,8
    mov rax, 7
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
