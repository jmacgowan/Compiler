global _start
_start:
    mov rax, 0
    push rax
    mov rax, 6
    push rax
; yooo
    push QWORD [rsp + 0]

    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    pop rdi
    syscall
