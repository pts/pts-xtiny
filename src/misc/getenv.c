#include <xtiny.h>

char *getenv(const char *name) {
  const char *p;
  for (p = name; *p != '\0' && *p != '='; ++p) {}
  if (environ && *name != '\0' && *p == '=') {
    char **e;
    const size_t len = p + 1 - name;
    for (e = environ; *e && 0 != memcmp(*e, name, len); ++e) {}
    if (*e) return *e + len;
  }
  return NULL;
}
