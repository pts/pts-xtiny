/*
 * addrnd.c: additive random stream generator
 * by pts@fazekas.hu at Wed Sep 12 21:31:41 CEST 2012
 *
 * This is a very fast random number generator, writing random bytes to
 * stdout. Takes the random seed from 220 bytes in the beginning of the
 * file specified in argv[1], or "/dev/urandom" as the default.
 *
 * Compilation with xtiny: xtiny gcc -W -Wall -Werror -o addrnd addrnd.c
 *
 * Compilation without xtiny: gcc -W -Wall -Werror -O2 -s -o addrnd addrnd.c
 *
 * Uses unpublished additive random generator algorithm by G.J. Mitchell and
 * D.P. Moore (1958), published as Algorithm A in subsection 3.2.2 of TAOCP.
 * See more here: https://en.wikipedia.org/wiki/Lagged_Fibonacci_generator ,
 * it's used with parameters {j = 24, k = 55}.
 */

#if __XTINY__
#include <xtiny.h>
#else
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#define sys_exit _exit
#endif

/* The real abort() call adds more dependencies, e.g. on kill(). */
static void __attribute__((__noreturn__)) my_abort(void) {
  sys_exit(104);
}

int main(int argc, char **argv) {
  int fd;
  int32_t y[55];
  int32_t w[8192];  /* 64K. */
  int32_t *p;
  register int j, k;
  int i, m;
  (void)argc;
  fd = open(argv[1] != NULL ? argv[1] : "/dev/urandom", O_RDONLY, 0);
  if (fd < 0) my_abort();
  y[0] = 0;  /* Pacify gcc-4.4, it thinks y is used uninitialized. */
  for (j = 0; j < 55 * 4; j += k) {  /* Read 55 * 4 bytes of random seed. */
    k = read(fd, j + (char*)y, 55 * 4 - j);
    if (k <= 0) my_abort();
  }
  close(fd);
  for (j = 0; j < 55 && (y[j] & 1) == 0; ++j) {}
  if (j == 55) y[0] |= 1;  /* Make sure not all numbers are even. */

  j = 24;
  k = 55;
  for (;;) {
    for (p = w; p != w + sizeof w / sizeof w[0]; ++p) {  /* Generate w. */
      *p = y[k - 1] +=  y[j - 1];
      if (--j == 0) j = 55;
      if (--k == 0) k = 55;
    }
    for (i = 0; i + 0U < sizeof w; i += m) {  /* Write w to stdout. */
      m = write(1, i + (char*)w, sizeof w - i);
      if (m <= 0) my_abort();
    }
  }
  return 0;
}
