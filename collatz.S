.global main
main:
	call readi32

loop:
	call writei32
	cmp $1, %eax
	jz end
	test $1, %eax
	jnz odd_step
	shr $1, %eax
	jmp loop

odd_step:
	mov %eax, %ebx
	shl $1, %eax
	jo end_error
	add %ebx, %eax
	jo end_error
	add $1, %eax
	jno loop

end_error:
	mov $-1, %eax
	call writei32
end:
	call finish
