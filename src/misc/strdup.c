#include <xtiny.h>

void *__xtiny_malloc(size_t size);

char *strdup(const char *s) {
  const size_t l = strlen(s) + 1;
  char *d = __xtiny_malloc(l);
  if (!d) return NULL;
  return memcpy(d, s, l);
}
