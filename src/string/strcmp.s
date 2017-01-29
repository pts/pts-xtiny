/* From dietlibc-0.33. */
.align 0
.global strcmp
.type	strcmp,@function
.weak	strcoll
.type	strcoll,@function

strcoll:
strcmp:
	movl 	4(%esp), %ecx
	movl 	8(%esp), %edx
	xorl	%eax, %eax
.Lloop:					/* Schleifenanfang liegt genau auf Modulanfang + 0x10, damit alignbar */
	movb 	(%ecx), %al
	cmpb	(%edx), %al
	jnz	.Ldiff
	incl 	%edx
	incl 	%ecx
	testb	%al, %al
	jnz	.Lloop
	ret
.Ldiff:
	movzbl	(%edx), %ecx
	subl	%ecx, %eax		/* (unsigned char)*p - (unsigned char)*q, so wie die Original libc */
	ret				/* und ohne Überlaufprobleme: */
					/* (int) ((signed char)c - (signed char)d) != (int)(signed char) ((unsigned char)c - (unsigned char)d) */
					/* c = x', d = 'e': left expression: -129, right expression: 127 */
	.size	strcmp, .-strcmp
