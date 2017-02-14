#include <xtiny.h>

void *__xtiny_malloc(size_t size);
void __xtiny_free(void *ptr);

void *realloc_grow(void *ptr, size_t old_size, size_t new_size) {
  if (new_size > old_size) {
    void *new_ptr = __xtiny_malloc(new_size);
    if (new_ptr) memcpy(new_ptr, ptr, old_size);
    __xtiny_free(ptr);
    ptr = new_ptr;
  }
  return ptr;
}
