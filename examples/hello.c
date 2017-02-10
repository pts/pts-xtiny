/* $ xtiny gcc -s -O2 -W -Wall -Wextra -o hello.xtiny hello.c
 * $ xstatic gcc -s -O2 -W -Wall -Wextra -o hello.xstatic hello.c
 * $ gcc -m32 -s -O2 -W -Wall -Wextra -o hello.dynamic hello.c
 * $ ls -ld hello.{xstatic,xtiny,dynamic}
 * -rwxr-xr-x 1 pts eng 5540 Feb  9 17:50 hello.dynamic
 * -rwxr-xr-x 1 pts pts 7340 Feb  9 17:49 hello.xstatic
 * -rwxr-xr-x 1 pts pts  292 Feb  9 17:49 hello.xtiny
 */

#ifdef __XTINY__
#include <xtiny.h>
#else
#include <stdio.h>
#endif

int main(int argc, char **argv) {
  (void)argc; (void)argv;
  puts("Hello, World!");
  return 0;
}
