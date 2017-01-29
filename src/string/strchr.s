/* From dietlibc-0.33. */
.align 0
.type   strchr,@function
.global strchr
.weak index
.type index,@function

index:
strchr:
	movl	4(%esp),%ecx
	movb	8(%esp),%dl
.Lloop:
	movb	(%ecx),%al
	cmpb	%al,%dl
	jz .Lfound
	incl	%ecx
	testb	%al,%al
	jnz .Lloop
	xorl	%ecx,%ecx
.Lfound:
	movl	%ecx,%eax
	ret
	.size	strchr, .-strchr
