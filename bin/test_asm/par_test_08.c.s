	.file	"par_test_08.c"
	.text
	.globl	main
	.type	main, @function
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$19, -8(%rbp)
	cmpl	$1, -8(%rbp)
	je	.L2
	cmpl	$2, -8(%rbp)
	je	.L3
	cmpl	$3, -8(%rbp)
	je	.L4
	cmpl	$4, -8(%rbp)
	je	.L5
	cmpl	$5, -8(%rbp)
	je	.L6
	cmpl	$6, -8(%rbp)
	je	.L7
	cmpl	$7, -8(%rbp)
	je	.L8
	cmpl	$8, -8(%rbp)
	je	.L9
	cmpl	$9, -8(%rbp)
	je	.L10
	cmpl	$10, -8(%rbp)
	je	.L11
	cmpl	$11, -8(%rbp)
	je	.L12
	cmpl	$12, -8(%rbp)
	je	.L13
	cmpl	$13, -8(%rbp)
	jne	.L14
	movl	$13, %eax
	jmp	.L27
.L14:
	movl	$0, %eax
	jmp	.L27
.L13:
	movl	$12, %eax
	jmp	.L27
.L12:
	movl	$11, %eax
	jmp	.L27
.L11:
	movl	$10, %eax
	jmp	.L27
.L10:
	movl	$9, %eax
	jmp	.L27
.L9:
	movl	$8, %eax
	jmp	.L27
.L8:
	movl	$7, %eax
	jmp	.L27
.L7:
	movl	$6, %eax
	jmp	.L27
.L6:
	movl	$5, %eax
	jmp	.L27
.L5:
	movl	$4, %eax
	jmp	.L27
.L4:
	movl	$3, %eax
	jmp	.L27
.L3:
	movl	$2, %eax
	jmp	.L27
.L2:
	movl	$1, %eax
.L27:
	movl	%eax, -4(%rbp)
	nop
	popq	%rbp
	ret
	.size	main, .-main
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
