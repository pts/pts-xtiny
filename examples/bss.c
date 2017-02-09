#ifdef __XTINY__
#include <xtiny.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

char buf[43];

#ifdef __XTINY__
void _start() {
#else
int main() {
#endif
  static const char msg[15] = "Hello, World!\n";
  (void)!write(1, msg, sizeof msg);
  (void)!read(0, buf, sizeof buf);
  exit(0);
}
