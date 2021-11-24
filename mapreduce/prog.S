.global main

.text

// std::transform_reduce([array], mapper, reducer, initial)
// -->
// accumulator = initial
// for a in array:
//    accumulator = reducer(accumulator, mapper(a))
// return accumulator

square:
  //   [ return-address arg1 arg2..  ]
  //     ^ esp
  mov 4(%esp), %eax
  imul %eax, %eax
  ret

sum:
  mov 4(%esp), %eax
  add 8(%esp), %eax
  ret

// scratch / caller-saved: %eax %ecx %edx
// calee-saved: %ebx %esi %edi %ebp %esp

mapreduce:
  // oldebp  (4)ret (8)array, (12)size, (16)mapper, (20)reducer, (24)initial
  // ^ebp
  push %ebp
  mov %esp, %ebp
  push %ebx
  mov 24(%ebp), %ebx
  mov 12(%ebp), %ecx
  test %ecx, %ecx
  jz exit
loop:
  mov 8(%ebp), %edx
  push (%edx)
  //mov 16(%ebp), %edx
  //call *%edx
  call *16(%ebp)
  add $4, %esp
  push %eax
  push %ebx
  mov 20(%ebp), %edx
  call *%edx
  mov %eax, %ebx
  add $8, %esp
  add $4, 8(%ebp)
  sub $1, 12(%ebp)  // size--
  jnz loop
exit:
  mov %ebx, %eax
  pop %ebx
  pop %ebp
  ret

main:
  push $0
  push $sum
  push $square
  push $3
  push $array
  call mapreduce
  call writei32
  add $4*5, %esp
  call finish

.data
array:
  .int 3
  .int 5
  .int 22
