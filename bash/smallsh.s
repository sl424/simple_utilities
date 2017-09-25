	.file	"smallsh.c"
	.section	.rodata
.LC0:
	.string	"sigchld"
.LC1:
	.string	"exit"
.LC2:
	.string	"status"
.LC3:
	.string	"cd"
.LC4:
	.string	"fork"
.LC5:
	.string	"sigint"
.LC6:
	.string	"background pid is:"
.LC7:
	.string	"%s %d\n"
.LC8:
	.string	"sigprocmask"
.LC9:
	.string	"terminated by signal"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$672, %rsp
	movl	%edi, -660(%rbp)
	movq	%rsi, -672(%rbp)
	movq	$0, -8(%rbp)
	movq	$0, -24(%rbp)
	movl	$0, -372(%rbp)
	movl	$0, -376(%rbp)
	movl	$0, -380(%rbp)
	movl	$1, -12(%rbp)
	movl	$1, %edi
	call	dup
	movl	%eax, -28(%rbp)
	movl	$0, %edi
	call	dup
	movl	%eax, -32(%rbp)
	movl	$-1, -388(%rbp)
	movl	$-1, -392(%rbp)
	movl	$0, -396(%rbp)
.L22:
	movl	$0, -384(%rbp)
	movl	-28(%rbp), %eax
	movl	$1, %esi
	movl	%eax, %edi
	call	dup2
	movl	-32(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	dup2
	movl	-388(%rbp), %eax
	cmpl	$-1, %eax
	je	.L2
	movl	-388(%rbp), %eax
	movl	%eax, %edi
	call	close
	movl	$-1, -388(%rbp)
.L2:
	movl	-392(%rbp), %eax
	cmpl	$-1, %eax
	je	.L3
	movl	-392(%rbp), %eax
	movl	%eax, %edi
	call	close
	movl	$-1, -392(%rbp)
.L3:
	cmpq	$0, -8(%rbp)
	je	.L4
	movl	$0, -16(%rbp)
	jmp	.L5
.L6:
	movl	-16(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	free
	addl	$1, -16(%rbp)
.L5:
	movl	-372(%rbp), %eax
	subl	$1, %eax
	cmpl	-16(%rbp), %eax
	jg	.L6
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	$0, -8(%rbp)
	movl	$0, -372(%rbp)
.L4:
	leaq	-368(%rbp), %rax
	addq	$8, %rax
	movq	%rax, %rdi
	call	sigemptyset
	movq	$handle_SIGCHLD, -368(%rbp)
	movl	$0, -232(%rbp)
	leaq	-368(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$17, %edi
	call	sigaction
	cmpl	$-1, %eax
	jne	.L7
	movl	$.LC0, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L7:
	leaq	-384(%rbp), %rdx
	leaq	-372(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	read_command
	movq	%rax, -8(%rbp)
	movl	-372(%rbp), %eax
	cmpl	$1, %eax
	je	.L8
	movl	-372(%rbp), %eax
	cmpl	$1, %eax
	jle	.L8
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	cmpb	$35, %al
	je	.L8
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L9
	call	cleanup
	movl	$0, -12(%rbp)
	jmp	.L8
.L9:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L10
	leaq	-380(%rbp), %rax
	movq	%rax, %rdi
	call	status
	jmp	.L8
.L10:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$.LC3, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L11
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	changewd
	jmp	.L8
.L11:
	call	fork
	movl	%eax, -36(%rbp)
	cmpl	$-1, -36(%rbp)
	jne	.L12
	movl	$.LC4, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L12:
	cmpl	$0, -36(%rbp)
	jne	.L13
	leaq	-208(%rbp), %rax
	addq	$8, %rax
	movq	%rax, %rdi
	call	sigemptyset
	movl	$0, -72(%rbp)
	movl	-384(%rbp), %eax
	testl	%eax, %eax
	je	.L14
	movq	$1, -208(%rbp)
	jmp	.L15
.L14:
	movq	$0, -208(%rbp)
.L15:
	leaq	-208(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$2, %edi
	call	sigaction
	cmpl	$-1, %eax
	jne	.L16
	movl	$.LC5, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L16:
	movl	-372(%rbp), %eax
	movl	%eax, -376(%rbp)
	leaq	-384(%rbp), %r8
	leaq	-392(%rbp), %rdi
	leaq	-388(%rbp), %rcx
	leaq	-396(%rbp), %rdx
	leaq	-376(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%r8, %r9
	movq	%rdi, %r8
	movq	%rax, %rdi
	call	openIO
	movq	%rax, -24(%rbp)
	movl	-396(%rbp), %eax
	testl	%eax, %eax
	jne	.L8
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	execvp
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	perror
	movl	$1, %edi
	call	exit
.L13:
	leaq	-208(%rbp), %rax
	addq	$8, %rax
	movq	%rax, %rdi
	call	sigemptyset
	movl	$0, -72(%rbp)
	movq	$1, -208(%rbp)
	leaq	-208(%rbp), %rax
	movl	$0, %edx
	movq	%rax, %rsi
	movl	$2, %edi
	call	sigaction
	cmpl	$-1, %eax
	jne	.L17
	movl	$.LC5, %edi
	call	perror
	movl	$1, %edi
	call	exit
.L17:
	movl	-384(%rbp), %eax
	testl	%eax, %eax
	je	.L18
	movl	-36(%rbp), %eax
	movl	%eax, %edx
	movl	$.LC6, %esi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	jmp	.L8
.L18:
	leaq	-656(%rbp), %rax
	movq	%rax, %rdi
	call	sigemptyset
	leaq	-528(%rbp), %rax
	movq	%rax, %rdi
	call	sigemptyset
	leaq	-528(%rbp), %rax
	movl	$17, %esi
	movq	%rax, %rdi
	call	sigaddset
	leaq	-656(%rbp), %rdx
	leaq	-528(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	sigprocmask
	leaq	-656(%rbp), %rdx
	leaq	-528(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	sigprocmask
	cmpl	$-1, %eax
	jne	.L19
	movl	$.LC8, %edi
	call	perror
.L19:
	leaq	-380(%rbp), %rcx
	movl	-36(%rbp), %eax
	movl	$0, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	waitpid
	movl	-380(%rbp), %eax
	andl	$127, %eax
	addl	$1, %eax
	sarb	%al
	testb	%al, %al
	jle	.L20
	movl	$2048, %edi
	call	malloc
	movq	%rax, -48(%rbp)
	movl	-380(%rbp), %eax
	andl	$127, %eax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, %r8d
	movl	$.LC9, %ecx
	movl	$.LC7, %edx
	movl	$2048, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
	movq	-48(%rbp), %rax
	movl	$2048, %edx
	movq	%rax, %rsi
	movl	$2, %edi
	call	write
	movq	stdout(%rip), %rax
	movq	%rax, %rdi
	call	fflush
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free
.L20:
	leaq	-656(%rbp), %rdx
	leaq	-528(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	sigprocmask
	cmpl	$-1, %eax
	jne	.L8
	movl	$.LC8, %edi
	call	perror
.L8:
	cmpl	$0, -12(%rbp)
	jne	.L22
	movl	$0, %edi
	call	exit
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 6.2.1 20160830"
	.section	.note.GNU-stack,"",@progbits
