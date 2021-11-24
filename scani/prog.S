  .global main
parse:
  // [ret] format A B C D .. 
  push %ebx
  mov 8(%esp), %ebx
  mov $12, %ecx
  push %esi
loop:
  cmpb $'\0', (%ebx)
  jz exit
  cmpb $'\l', (%ebx)
  mov (%esp, %ecx), %esi
  jz parse_l
parse_d:
  call readi64 // eax: edx
  mov %eax, (%esi)
  mov %edx, 4(%esi)
  jmp endloop
parse_l:
  call readi32
  mov %eax, (%esi)
endloop:
  add $4, %ecx
  jmp loop
exit:
  pop %esi
  pop %ebx
  ret
main:
  push $D
  push $C
  push $B
  push $A
  push $format
  call parse

  mov C, %eax
  call writei32
  call finish
format:
  .ascii "dldl\0"
C:
  .int 0
B:
  .quad 0
A: // 0x400000
  .int 0
D: // 0x400004
  .quad 0
