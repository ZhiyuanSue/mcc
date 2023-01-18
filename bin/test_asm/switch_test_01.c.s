	.file	"switch_test_01.c"
	.text
	.globl	abcdefg
	.bss
	.align 4
	.type	abcdefg, @object
	.size	abcdefg, 4
abcdefg:
	.zero	4
	.text
	.globl	tttst
	.type	tttst, @function
tttst:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$1, -4(%rbp)
	addl	$1, -4(%rbp)
	nop
	movl	$0, %eax
	popq	%rbp
	ret
	.size	tttst, .-tttst
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
