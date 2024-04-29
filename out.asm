global _start
_start:
    mov rax, 32
    push rax
    mov rax, 5
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jg condition_true
    jmp condition_end
condition_true:
    mov rax, 5
    push rax
    mov rax, 4
    push rax
    mov rax, 7
    push rax
    pop rax
    pop rbx
    imul rax, rbx
    push rax
    mov rax, 60
    pop rdi
    syscall
    jmp if_end
condition_end:
    mov rax, 4
    push rax
    mov rax, 60
    pop rdi
    syscall
if_end:
    mov rax, 8
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
