    .global main
main:
    // edx:eax
    call readi64
    mov %eax, %ebx
    mov %edx, %ecx    
    call readi32
    xor %edx, %edx
    test %eax, %eax
    jns addition
substract:
    not %edx
addition:
    add %ebx, %eax
    adc %ecx, %edx
    call writei64
    call finish
