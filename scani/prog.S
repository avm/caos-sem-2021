  .global main
parse:
  // [ret] format A B C D .. 
  push %ebx
  mov 8(%esp), %ebx
  mov $0, %ecx
  push %esi
loop:
  cmpb $0, (%ebx, %ecx)
  jz exit
  mov 16(%esp, %ecx, 4), %esi
  cmpb $'\d', (%ebx, %ecx)
  jz parse_d
parse_l:
  call readi64 // eax: edx
  mov %eax, (%esi)
  mov %edx, 4(%esi)
  jmp endloop
parse_d:
  call readi32
  mov %eax, (%esi)
endloop:
  inc %ecx
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

  mov A, %eax
  call writei32
  mov B, %eax
  mov B+4, %edx
  call writei64
  mov C, %eax
  call writei32
  mov D, %eax
  mov D+4, %edx
  call writei64
  call finish
format:
  .ascii "dldl\0"

  .data
C:
  .int 0
B:
  .quad 0
A: // 0x400000
  .int 0
D: // 0x400004
  .quad 0
