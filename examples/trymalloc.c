/* by pts@fazekas.hu at Mon Jan 30 15:18:46 CET 2017 */
#ifdef __XTINY__
#define NO_FREE 1
#endif
typedef unsigned long size_t;
extern void *malloc(size_t size);
extern void free(void *ptr);
extern void *calloc(size_t nmemb, size_t size);
extern void *realloc(void *ptr, size_t size);
extern void abort();

int main(int argc, char **argv) {
  enum { N = 60000 };  /* TODO(pts): Why is 70000 too much for valgrind? */
  char *p[N];
  unsigned i;
  (void)argc; (void)argv;
  for (i = 0; i < N; ++i) {
    p[i] = malloc(33);
    p[i][0] = 'X';
    p[i][32] = 'Y';
  }
#ifndef NO_FREE
  for (i = 0; i < N; ++i) {
    p[i] = realloc(p[i], 77);
    if (p[i][32] != 'Y') abort();
    p[i][0] = 'A';
    p[i][76] = 'B';
  }
#endif
  for (i = 0; i < N; ++i) {
#ifndef NO_FREE
    free(p[i]);
#endif
    p[i] = calloc(3, 11);
    if (p[i][0] != 0) abort();
    if (p[i][32] != 0) abort();
    p[i][32] = 'Z';
  }
#ifndef NO_FREE
  for (i = 0; i < N; ++i) {
    free(p[i]);
  }
#endif
  return 0;
}
