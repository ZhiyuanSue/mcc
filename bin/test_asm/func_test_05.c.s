	.file	"func_test_05.c"
	.text
	.globl	test
	.type	test, @function
test:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	movsd	%xmm0, -16(%rbp)
	movl	%esi, -8(%rbp)
	nop
	popq	%rbp
	ret
	.size	test, .-test
	.globl	abc
	.type	abc, @function
abc:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %eax
	popq	%rbp
	ret
	.size	abc, .-abc
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
