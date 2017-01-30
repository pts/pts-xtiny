#ifdef __XTINY__
#include <xtiny.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

__attribute__((constructor)) void myctor1(void) {
  (void)!write(1, "MYCTOR1\n", 8);
}

__attribute__((constructor)) static void myctor2(void) {
  (void)!write(1, "MYCTOR2\n", 8);
}

__attribute__((destructor)) static void mydtor1(void) {
  (void)!write(1, "MYDTOR1\n", 8);
}

__attribute__((destructor)) static void mydtor2(void) {
  (void)!write(1, "MYDTOR2\n", 8);
}

static void myatexit(void) {
  (void)!write(1, "MYATEXIT\n", 9);
}

int main(int argc, char **argv) {
  (void)argc; (void)argv;
  (void)!write(1, "MAIN\n", 5);
  atexit(myatexit);
  return 0; /*(int)__init_array_start;*/ 
}
