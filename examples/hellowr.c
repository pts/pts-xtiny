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
  (void)!write(1, msg, sizeof msg);
  exit(0);
}
