#include <xtiny.h>

char *strstr(const char *h, const char *n) {
  size_t nlen;
  const char c = *n++;
  if (!c) return (char*)h;
  nlen = strlen(n);
  while ((h = strchr(h, c)) && 0 != memcmp(++h, n, nlen)) {}
  return (char*)h;
}
