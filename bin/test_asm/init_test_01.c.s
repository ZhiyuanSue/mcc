	.file	"init_test_01.c"
	.text
	.globl	a
	.data
	.align 8
	.type	a, @object
	.size	a, 8
a:
	.long	1
	.long	2
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	1
	.globl	c
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.long	1
	.globl	tmp
	.align 8
	.type	tmp, @object
	.size	tmp, 8
tmp:
	.long	1
	.long	2
	.globl	w
	.align 32
	.type	w, @object
	.size	w, 64
w:
	.long	1
	.zero	8
	.zero	4
	.long	2
	.long	3
	.long	4
	.long	5
	.zero	16
	.long	7
	.long	8
	.zero	4
	.zero	4
	.globl	x
	.align 16
	.type	x, @object
	.size	x, 16
x:
	.zero	8
	.long	0
	.long	1073741824
	.globl	y
	.align 32
	.type	y, @object
	.size	y, 48
y:
	.long	1
	.long	3
	.long	5
	.long	2
	.long	4
	.long	6
	.long	3
	.long	5
	.long	7
	.zero	12
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
