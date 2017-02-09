#include <xtiny.h>
void _start() {
  static const char msg[15] = "Hello, World!\n";
  (void)!write(1, msg, sizeof msg);
  exit(0);
}
