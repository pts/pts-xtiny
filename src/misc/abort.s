	.text
	.globl	abort
	.type	abort, @function
abort:
	/* pushl %ebx */
	movl	$20, %eax  /* __NR_getpid */
	int $0x80
	xchgl	%eax, %ebx
	movl	$37, %eax  /* __NR_kill */
	movl	$6, %ecx  /* SIGABRT */
	int $0x80
	/* popl	%ebx */
	/* No need to ret, doesn't return (with the default handler): ret */
	.size	abort, .-abort
