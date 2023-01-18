	.file	"linkage_test_02.c"
	.text
	.local	d
	.comm	d,8,8
	.section	.rodata
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.zero	4
	.text
	.globl	testa
	.type	testa, @function
testa:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	%edi, -4(%rbp)
	nop
	popq	%rbp
	ret
	.size	testa, .-testa
	.local	e.1922
	.comm	e.1922,4,4
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
