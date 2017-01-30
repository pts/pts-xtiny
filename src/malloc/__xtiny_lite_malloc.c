#include <xtiny.h>
#define BRK(n) sys_brk((void*)(n))
#undef weak_alias
#define weak_alias(old, new) \
        extern __typeof(old) new __attribute__((weak, alias(#old)))

#define ALIGN 16

/* This function returns true if the interval [old,new]
 * intersects the 'len'-sized interval below &libc.auxv
 * (interpreted as the main-thread stack) or below &b
 * (the current stack). It is used to defend against
 * buggy brk implementations that can cross the stack. */

static int traverses_stack_p(size_t old, size_t new)
{
	const size_t len = 8<<20;
	size_t a, b;

/* Typically the stack is at 0xbf?????? or 0xff?????? */
#ifndef __XTINY__
	b = (size_t)libc.auxv;
	a = b > len ? b-len : 0;
	if (new>a && old<b) return 1;
#endif

	b = (size_t)&b;
	a = b > len ? b-len : 0;
	if (new>a && old<b) return 1;

	return 0;
}

void *__xtiny_lite_malloc(size_t n)
{
	static size_t brkv, base_brkv, heap_used;
	size_t g, h, align;
	void *result;

	if (!n) n++;
	for (align = 1; align < n && align < ALIGN; align <<= 1) {}
	align = -(size_t)heap_used & (align-1);
	if (n <= brkv - heap_used - align) {
		result = (void*)(heap_used += align);
		heap_used += n;
		return result;
	}

	if (!base_brkv) {
		/* We must call BRK(0) which returns the start of brk space.
		 * We can't assume that brk space starts at &_end (provided
		 * by the linker script), because brk space is randomized:
		 * $ cat /proc/sys/kernel/randomize_va_space
		 * 2
		 * See https://www.kernel.org/doc/Documentation/sysctl/kernel.txt
		 * Please note that randomization doesn't happen with valgrind,
		 * but it happens natively and with strace.
		 */
		base_brkv = (size_t)BRK(0);
		base_brkv += -base_brkv & (PAGE_SIZE-1);
		heap_used = brkv = base_brkv;
		g = PAGE_SIZE * 2;
		align = 0;  /* Already properly aligned. */
	} else {
		g = brkv - base_brkv;
	}
	h = heap_used - base_brkv;
	if (n > SIZE_MAX - 16 - h) goto err;
	h += n + align;
	while (h > g) {
		g += g >> 1;
	}
	if (g > SIZE_MAX - PAGE_SIZE + 1 - base_brkv) goto err;
	g += -g & (PAGE_SIZE-1);
	g += base_brkv;
	if (traverses_stack_p(brkv, g) || (size_t)BRK(g) != g) goto err;
    	result = (void*)(heap_used += align);
        memset((void*)brkv, '\0', g - brkv);  /* TODO(pts): Inline memset? */
	brkv = g;
	heap_used += n;
	return result;

err:
	errno = ENOMEM;
	return 0;
}

weak_alias(__xtiny_lite_malloc, malloc);
weak_alias(__xtiny_lite_malloc, __malloc0);

#if 0
void *malloc(size_t n) {
  return __xtiny_lite_malloc(n);
}
void *__malloc0(size_t n) {
  return __xtiny_lite_malloc(n);
}
#endif
