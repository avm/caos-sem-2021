.global main
main:
    mov $0, %ebx
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
    cmp $0, %eax
    // %eax - 0
    jg update_left
    jl update_right
    mov %edx, %ebx
    jmp end
update_left:
    mov %edx, %ebx
    jmp loop
update_right:
    mov %edx, %ecx
    jmp loop
end:
    mov %ebx, %eax
    call writei32
    call finish

    // ...
    
