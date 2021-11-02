.global main
main:
    xor %ebx, %ebx
    mov $101, %ecx
   // ans in [ebx, ecx) 
loop:
    mov %ecx, %edx
    sub %ebx, %edx
    cmp $1, %edx
    je end
    shr $1, %edx
    add %ebx, %edx
    mov %edx, %eax
    call writei32
    call readi32
    test %eax, %eax
    jg update_left
    jl update_right
    mov %edx, %ebx
end:
    mov %ebx, %eax
    call writei32
    call finish
update_left:
    mov %edx, %ebx
    jmp loop
update_right:
    mov %edx, %ecx
    jmp loop
