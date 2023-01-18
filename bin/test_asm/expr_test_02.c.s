	.file	"expr_test_02.c"
	.text
	.globl	t
	.type	t, @function
t:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	-24(%rbp), %eax
	movl	%eax, -36(%rbp)
	nop
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L2
	call	__stack_chk_fail@PLT
.L2:
	leave
	ret
	.size	t, .-t
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
