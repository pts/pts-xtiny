/* From gcc-4.6.3. */ 
.align 0
	.globl	memcpy
	.type	memcpy, @function
memcpy:
	pushl	%edi
	pushl	%esi
	movl	12(%esp), %eax
	movl	16(%esp), %esi
	movl	20(%esp), %ecx
	movl	%eax, %edi
	rep movsb
	popl	%esi
	popl	%edi
	ret
	.size	memcpy, .-memcpy
