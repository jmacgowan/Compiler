global _start
_start:
    mov rax, 0
    push rax
    mov rax, 6
    push rax
    mov rax, 3
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    cmp rbx, rax
    jg condition_true_0
    jmp condition_end_0
condition_true_0:
    mov rax, 5
    push rax
    mov rax, 60
    pop rdi
    syscall
    jmp if_end_0
condition_end_0:
    mov rax, 7
    push rax
    mov rax, 60
    pop rdi
    syscall
if_end_0:
    push QWORD [rsp + 16]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
