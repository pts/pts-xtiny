/* by pts@fazekas.hu at Tue Feb 14 17:38:01 CET 2017
 *
 * xtiny   gcc                        -W -Wall -Wextra -s -Os -o test_stat test_stat.c 
 * xtiny   gcc -D_FILE_OFFSET_BITS=64 -W -Wall -Wextra -s -Os -o test_stat test_stat.c 
 * xstatic gcc                        -W -Wall -Wextra -s -Os -o test_stat.xstatic test_stat.c
 * xstatic gcc -D_FILE_OFFSET_BITS=64 -W -Wall -Wextra -s -Os -o test_stat.xstatic test_stat.c
 */

#ifdef __XTINY__
#include <xtiny.h>
#if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
#  define stat stat64
#  define lstat lstat64
#endif
#else
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

void printhex(const char *prefix, long long v) {
  static const char hextable[] = "0123456789abcdef";
  char buf[80], *p, *q;
  const size_t prefix_size = strlen(prefix);
  if (prefix_size > 60) abort();
  p = buf;
  memcpy(p, prefix, prefix_size);
  p += prefix_size;
  q = p + 16;
  while (q != p) {
    *--q = hextable[v & 15];
    v >>= 4;
  }
  p += 16;
  *p++ = '\n';
  *p = '\0';
  (void)!write(1, buf, p - buf);
}
  

int main(int argc, char **argv) {
  (void)argc;
  for (++argv; *argv; ++argv) {
    struct stat st;
    (void)!write(1, *argv, strlen(*argv));
    (void)!write(1, "\n", 1);
#if 0
    st.__pad_end = 0xaabbccdd55667788ULL;
#endif
    if (lstat(*argv, &st) != 0) {
      (void)write(1, "  error\n", 8);
    } else {
      printhex("  st_dev:     ", st.st_dev);
      printhex("  st_ino:     ", st.st_ino);
      printhex("  st_mode:    ",  st.st_mode);
      printhex("  st_nlink:   ",  st.st_nlink);
      printhex("  st_uid:     ",  st.st_uid);
      printhex("  st_gid:     ",  st.st_gid);
      printhex("  st_rdev:    ",  st.st_rdev);
      printhex("  st_size:    ",  st.st_size);
      printhex("  st_blksize: ",  st.st_blksize);
      printhex("  st_blocks:  ",  st.st_blocks);
      printhex("  st_atime:   ",  st.st_atime);
      printhex("  st_mtime:   ",  st.st_mtime);
      printhex("  st_ctime:   ",  st.st_ctime);
    }
#if 0
    /* Verify canary. */
    if (st.__pad_end != 0xaabbccdd55667788ULL) abort();
#endif
  }
  return 0;
}
