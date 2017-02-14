#include <xtiny.h>

void *__xtiny_malloc0(size_t);

void *calloc(size_t m, size_t n) {
  if (n && m > (size_t)-1/n) {
    errno = ENOMEM;
    return 0;
  }
  return __xtiny_malloc0(n * m);
}
