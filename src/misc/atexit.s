	.text
	.globl	atexit
	.type	atexit, @function
atexit:
	cmpl	$ __xtiny_atexit_ret, __xtiny_atexit_hook
	jne	.L4
	movl	%eax, __xtiny_atexit_hook
	xorl	%eax, %eax
	ret
.L4:
	orl	$-1, %eax
	.type	__xtiny_atexit_ret, @function
__xtiny_atexit_ret:
	ret
	.size	 __xtiny_atexit_ret, .- __xtiny_atexit_ret
	.size	atexit, .-atexit

	/*.globl	__xtiny_atexit_hook*/
	.section	.fini_array,"aw"
	.align 4
	.type	__xtiny_atexit_hook, @object
	.size	__xtiny_atexit_hook, 4
__xtiny_atexit_hook:
	.long	 __xtiny_atexit_ret
