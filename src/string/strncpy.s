/* From dietlibc-0.33. */
.align 0
/*
  Copyright (C) 2002 Thomas M. Ogrisegg

  This is free software. You can redistribute and
  modify it under the terms of the GNU General Public
  Public License.

  strncpy.S
    i386 assembler implementation of strncpy(3)
*/

.globl strncpy
.type strncpy,@function

strncpy:
	pushl %esi
	pushl %edi
	movl %esp, %ecx
	movl  0x0c(%ecx), %edi
	movl  0x10(%ecx), %esi
	movl  0x14(%ecx), %ecx

	movl %edi, %edx
	cld
.Lloop:
	dec %ecx
	js .Lout
	lodsb
	stosb
	or %al, %al
	jnz .Lloop
	repnz stosb
.Lout:
	movl %edx, %eax
	popl %edi
	popl %esi
	ret
	.size	strncpy, .-strncpy
