/* From dietlibc-0.33 */
.align 0
.global memccpy
.type   memccpy,@function
memccpy:
		pushl %esi
		pushl %edi

		movl %esp, %ecx
		movl 0x0c(%ecx), %edi
		movl 0x10(%ecx), %esi
		movl 0x14(%ecx), %edx
		movl 0x18(%ecx), %ecx
		cld
		jecxz	.Lerr
.Lloop:
		lodsb
		stosb
		cmp %al, %dl
		jz .Lout
		decl %ecx
		jnz .Lloop
.Lerr:
		xorl %edi, %edi
.Lout:
		movl %edi, %eax
		popl %edi
		popl %esi
		ret
.Lende:
.size    memccpy,.Lende-memccpy
