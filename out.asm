global _start
_start:
    mov rax, 5
    push rax
    mov rax, 4
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jl condition_true_0
    jmp condition_end_0
condition_true_0:
    mov rax, 5
    push rax
    mov rax, 60
    pop rdi
    syscall
condition_end_0:
if_end_0:
    mov rax, 4
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
