#ifdef __XTINY__
#include <xtiny.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

#ifdef __XTINY__
void _start() {
#else
int main() {
#endif
  static const char msg[15] = "Hello, World!\n";
  int i = sizeof msg, got;
  while (i > 0) {
    got = write(1, msg, sizeof msg);
    if (got <= 0) break;
    i -= got;
  }
  exit(0);
}
