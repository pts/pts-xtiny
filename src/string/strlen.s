/* From gcc-4.6.3. */
.align 0
	.globl	strlen
	.type	strlen, @function
strlen:
	pushl	%edi
	movl	8(%esp), %edi
	xorl	%eax, %eax
	orl	$-1, %ecx
	repnz scasb
	popl	%edi
	notl	%ecx
	leal	-1(%ecx), %eax
	ret
	.size	strlen, .-strlen
