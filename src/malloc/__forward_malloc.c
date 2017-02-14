#include <xtiny.h>

/* Another .o file must define these (with any size):
 * char __forward_malloc_heap[1200000000] __attribute__((aligned(8)));
 * char *__forward_malloc_heap_end = __forward_malloc_heap + sizeof __forward_malloc_heap;
 *
 * Please note that Linux allocates only as much physical memory as used.
 */
extern char __forward_malloc_heap[];
extern char *__forward_malloc_heap_end;
char * __forward_malloc_heap_pos = __forward_malloc_heap;

void *__forward_malloc(size_t size) {
  char *result;
  if ((ssize_t)size <= 0) goto err;  /* Disallow 0, check for overflow. */
  size += -size & 7;  /* Align to 8-byte boundary. */
  /* Report out-of-memory error. */
  if (size > (size_t)(__forward_malloc_heap_end -
      __forward_malloc_heap_pos)) { err:
    errno = ENOMEM;
    return 0;
  }
  result = __forward_malloc_heap_pos;
  __forward_malloc_heap_pos += size;
  return result;
}

void *__xtiny_malloc(size_t size) __attribute__((alias("__forward_malloc")));
/* Blocks returned by __forward_malloc are always 0-initialized. */
void *__xtiny_malloc0(size_t size) __attribute__((alias("__forward_malloc")));
