.align 0
	.globl	memchr
	.type	memchr, @function
memchr:
	pushl	%edi
	movl	16(%esp), %ecx
	xorl	%eax, %eax
	testl	%ecx, %ecx
	je	.L2
	movl	8(%esp), %edx
	movl	12(%esp), %eax
	movl	%edx, %edi
	/* cld */
	repne scasb
	je .L1
	xor %edi, %edi
	incl %edi
.L1:	decl %edi
	movl	%edi, %eax
.L2:
	popl	%edi
	ret
	.size	memchr, .-memchr
