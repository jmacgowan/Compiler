global _start
_start:
    mov rax, 6
    push rax
    mov rax, 0
    push rax
loop_start_1:
    mov rax, 5
    push rax  ; Push value 5 onto the stack

    pop rbx   ; Pop the value from the stack into rbx
    cmp rbx, rax
    jge condition_end_1

condition_true_1:
    mov rax, 8
    push rax
    mov [rsp + 16], rax   ; Store the value at the calculated offset
    add rsp, 8            ; Adjust the stack pointer after storing

    push QWORD [rsp + 8]  ; Push the updated value from the stack top

    mov rax, 1
    push rax
    pop rax
    pop rbx
    add rax, rbx
    push rax
    pop rax
    mov [rsp + 16], rax   ; Store the value at the calculated offset
    jmp loop_start_1
condition_end_1:
    push QWORD [rsp + 24]

    mov rax, 60
    pop rdi
    syscall
