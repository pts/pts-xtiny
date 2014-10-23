/* From gcc-4.6.3. */
.align 0
	.globl	memset
	.type	memset, @function
memset:
	pushl	%edi
	movl	8(%esp), %edx
	movb	12(%esp), %al
	movl	16(%esp), %ecx
	movl	%edx, %edi
	rep stosb
	movl	%edx, %eax
	popl	%edi
	ret
	.size	memset, .-memset
