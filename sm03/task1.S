#include <sys/syscall.h>
	.global main
readSymb:
	//  caller-saved (scratch): eax ecx edx
	//  callee-saved:           ebx esi edi esp ebp
	push %ebp
	mov %esp, %ebp
	push %ebx
	.equ READ_FILENO, 0
	sub $4, %esp
	mov $SYS_read, %eax
	mov $READ_FILENO, %ebx
	mov %esp, %ecx
	mov $1, %edx
	// [   a b c d %ebx ret ]
	// 0   ^ esp
	int $0x80
	test %eax, %eax
	jng error
	movzb (%esp), %eax // a
	jmp exit
error:
	mov $-1, %eax
exit:
	add $4, %esp
	pop %ebx
	pop %ebp
	ret
/// input:
/// al - number
/// output:
/// eax - leading ones in ax
CountLeadingOnes:
        push %ebp
        mov %esp, %ebp

        xor %ecx, %ecx
clo_loop:
        shl $1, %al
        jnc clo_ret
        add $1, %ecx
        jmp clo_loop
clo_ret:   
        mov %ecx, %eax
        mov %ebp, %esp
        pop %ebp
        ret
main:
        // read so far corepoint
        xor %ebx, %ebx
        // unread trailing bytes count
        xor %esi, %esi
loop:
        call readSymb
        test %eax, %eax
        push %eax
        jl eof
	// b0, b0 < 128   0b0.......
        // b1, b2, .. bk  b1: 0b1110... b2..k: 0b10.....
        test %esi, %esi
        jz codepoint_begin
        and $0x3f, %eax
        // edi = 0 -> 25
        // 25 - edi
        shl $6, %ebx
        add %eax, %ebx
        sub $1, %esi
        jg loop
codepoint_done:
        mov %ebx, %eax
        call writei32
        xor %ebx, %ebx
        xor %edi, %edi
        jmp loop
codepoint_begin:
        mov %eax, %ebx
        call CountLeadingOnes
        test %eax, %eax
        jz codepoint_done
        sub $1, %eax
        // eax = 
        mov %eax, %esi
        jmp loop
eof:
	mov $0, %ebx
        mov $SYS_exit, %eax
        int $0x80
