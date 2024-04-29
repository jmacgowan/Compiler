global _start
_start:
    mov rax, 32
    push rax
    mov rax, 5
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jg condition_true_0
    jmp condition_end_0
condition_true_0:
condition_true_0:
    mov rax, 5
    push rax
    mov rax, 6
    push rax
    jmp if_end_0
condition_end_0:
    mov rax, 4
    push rax
    mov rax, 60
    pop rdi
    syscall
if_end_0:
    mov rax, 32
    push rax
    mov rax, 5
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jg condition_true_1
    jmp condition_end_1
condition_true_1:
condition_true_1:
    mov rax, 3
    push rax
    mov rax, 60
    pop rdi
    syscall
    jmp if_end_1
condition_end_1:
    mov rax, 8
    push rax
    mov rax, 60
    pop rdi
    syscall
if_end_1:
    mov rax, 11
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 8
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
