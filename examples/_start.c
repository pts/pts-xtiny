#define __NR_sys_exit 1

#define _syscall1_noreturn(name,type1,arg1) \
static __inline__ void __attribute__((noreturn)) name(type1 __##arg1) { \
long __res; \
__asm__ volatile ("int $0x80" \
  : "=a" (__res) \
  : "0" (__NR_##name),"b" ((long)(__##arg1))); \
__builtin_unreachable(); \
}

_syscall1_noreturn(sys_exit,int,exitcode)

extern int main(int argc, char **argv, char **envp);
extern char **environ;

/* Entry point of the program. Default: ld -e _start
 * On Linux i386: http://asm.sourceforge.net/articles/startup.html
 */
void _start(char *argv0) {
  char **a = &argv0;
  while (*a) a++;
  environ = a + 1;
  sys_exit(main(a - &argv0, &argv0, a + 1));
}  
