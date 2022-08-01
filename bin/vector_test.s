	.file	"vector.c"
	.text
	.globl	InitVEC
	.type	InitVEC, @function
InitVEC:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$24, %edi
	call	m_alloc@PLT
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	m_alloc@PLT
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-8(%rbp), %rax
	movq	$3, 16(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-8(%rbp), %rax
	leave
	ret
	.size	InitVEC, .-InitVEC
	.globl	VECinsert
	.type	VECinsert, @function
VECinsert:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L6
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	VECexpand
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	salq	$3, %rax
	addq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rax, (%rdx)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	leaq	1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 8(%rax)
	jmp	.L3
.L6:
	nop
.L3:
	leave
	ret
	.size	VECinsert, .-VECinsert
	.globl	VECremove
	.type	VECremove, @function
VECremove:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L13
	movq	-16(%rbp), %rax
	cmpq	-24(%rbp), %rax
	je	.L14
	jmp	.L11
.L12:
	movq	-8(%rbp), %rax
	movq	(%rax), %rcx
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -24(%rbp)
	salq	$3, %rax
	addq	%rax, %rcx
	movq	-8(%rbp), %rax
	movq	(%rax), %rsi
	movq	-16(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -16(%rbp)
	salq	$3, %rax
	leaq	(%rsi,%rax), %rdx
	movq	(%rcx), %rax
	movq	%rax, (%rdx)
.L11:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	cmpq	%rax, -24(%rbp)
	jb	.L12
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rdx
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	VECshrink
	jmp	.L7
.L13:
	nop
	jmp	.L7
.L14:
	nop
.L7:
	leave
	ret
	.size	VECremove, .-VECremove
	.globl	VECpopback
	.type	VECpopback, @function
VECpopback:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L19
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L20
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	leaq	-1(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 8(%rax)
	jmp	.L15
.L19:
	nop
	jmp	.L15
.L20:
	nop
.L15:
	popq	%rbp
	ret
	.size	VECpopback, .-VECpopback
	.globl	VECexpand
	.type	VECexpand, @function
VECexpand:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L26
	movq	-24(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	cmpq	%rax, %rdx
	jb	.L27
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	cmpq	$2, %rax
	ja	.L25
	movq	-24(%rbp), %rax
	movq	$3, 16(%rax)
.L25:
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	salq	$4, %rax
	movq	%rax, %rdi
	call	m_alloc@PLT
	movq	-24(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	leaq	(%rax,%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	m_free@PLT
	jmp	.L21
.L26:
	nop
	jmp	.L21
.L27:
	nop
.L21:
	leave
	ret
	.size	VECexpand, .-VECexpand
	.globl	VECshrink
	.type	VECshrink, @function
VECshrink:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L33
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	cmpq	$5, %rax
	jbe	.L34
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	cmpq	%rax, %rdx
	ja	.L35
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	shrq	%rax
	salq	$3, %rax
	movq	%rax, %rdi
	call	m_alloc@PLT
	movq	-24(%rbp), %rdx
	movq	%rax, (%rdx)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	leaq	0(,%rax,8), %rdx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	memcpy@PLT
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	shrq	%rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	m_free@PLT
	jmp	.L28
.L33:
	nop
	jmp	.L28
.L34:
	nop
	jmp	.L28
.L35:
	nop
.L28:
	leave
	ret
	.size	VECshrink, .-VECshrink
	.globl	DelVEC
	.type	DelVEC, @function
DelVEC:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L39
	movq	-8(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-8(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	VECremove
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	m_free@PLT
	jmp	.L36
.L39:
	nop
.L36:
	leave
	ret
	.size	DelVEC, .-DelVEC
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
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
