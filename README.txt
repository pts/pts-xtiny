README for pts-xtiny
^^^^^^^^^^^^^^^^^^^^
pts-xtiny is a collection of tools and a small and incomplete libc for
creating tiny (200-byte), statically linked ELF executables on Linux i386.

pts-xtiny was inspired by the tiny ELF assembly header in ``A Whirlwind
Tutorial on Creating Really Teensy ELF Executables for Linux''
(http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html). pts-xtiny
achieves almost the same, tiny file size as indicated there, but without the
programmer having to write any assembly code. The ELF file generation is
done using a GNU ld linker script.

FAQ
~~~

Q1. How large is a typical executable?
""""""""""""""""""""""""""""""""""""""
A typical Hello, World program is only 200 bytes:

  $ ./xtiny gcc examples/hellot.c
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 200 Oct 20 03:15 a.out

The hand-optimized assembly version, compiled from examples/hello.nasm, is
138 bytes long, 62 bytes shorter.

After the `gcc' in the command above almost all gcc flags are supported (and
will be passed to gcc). Examples of supported flags: -c, -S, -E, -M, -MM,
-W..., -f..., -m... .

If you forget to specify the `-Os' flag to make the compiler optimize for
file size, the `xtiny' wrapper will add it for you. Unless you specify another
`-O...' flag, the wrapper also adds some other relevant gcc flags to make
the output file even smaller.

Q2. Which compilers are supported?
""""""""""""""""""""""""""""""""""
gcc-4.4 and later, and clang-3.0 and later. Probably it would be possible to
make it work with gcc-4.3, gcc-4.2 and gcc-4.1 if there is interest.

You have to install the compiler separately. You can install it from package
(e.g. `sudo apt-get install gcc' on Debian-ish systems).

Q3. Which architectures are supported?
""""""""""""""""""""""""""""""""""""""
i386 (x86, 32-bit) only. The generated binaries will run on amd64 (x86_64)
Linux systems as well.

There is no need to specify the `-m32' flag for gcc, xtiny specifies it by
default.

Q4. Which operating systems are supported?
""""""""""""""""""""""""""""""""""""""""""
Linux only, and also FreeBSD in Linux emulation mode.

Q5. Do the executables link against glibc, eglibc, uClibc, dietlibc?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
No, they link statically against the xtiny libc only, which is included in
the pts-xtiny distribution. At runtime no external files are necessary to
run the executables.

Q6. The xtiny libc is very small, will you add function X?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
If X is a Linux system call, please send me a patch, and I'll add it.
Otherwise let's discuss first to avoid the extra work.

Your contributions are welcome and appreciated.

Q7. Which programming languages are supported?
""""""""""""""""""""""""""""""""""""""""""""""
Currently C and i386 assembly (as accepted by the GNU assembler) are
supported.

C++ may get added in the future, but there will be no exception handling, no
STL, and libstdc++ will not be available.

Q8. Can I use existing, precompiled libraries?
""""""""""""""""""""""""""""""""""""""""""""""
Most probably they won't work, because most of them need lots of functions
from the libc (which the xtiny libc doesn't have), and when they were
compiled, their .h files made some assumptions about the inner data
structers of that other libc, and these assumptions are most probobaly not
true for pts-xtiny. So even if it compiles, it will segfault.

Q9. Can I build and use shared libraries (.so files)?
"""""""""""""""""""""""""""""""""""""""""""""""""""""
The design of pts-xtiny fundamentally doesn't support that, so that will
never work.

Q10. What gcc flags are recommended?
""""""""""""""""""""""""""""""""""""
You don't have to specify -Os, -static or -s, these are automatically
specified by the `xtiny' wrapper script. Unless you speciy another
`-O...' flag, the wrapper also adds some other relevant gcc flags to make
the output file even smaller.

It's recommended that you specify some warning flags, like these: -W -Wall
-Wextra -Werror Wsystem-headers.

You may also want to specify -ansi or `-ansi -pedantic', but that usually
gives you only very little benefit in addition to the warning flags.

Q11. What are the dependencies to run pts-xtiny?
""""""""""""""""""""""""""""""""""""""""""""""""
* a Linux i386 or amd64 system
* gcc >=4.4 that can compile for the i386 target
* the GNU linker: the ld tool GNU Binutils (other linkers such as gold
  without linker script (`ld -T') support won't work)
* Python >=2.4 (This may get removed in the future when the `xtiny' wrapper
  script gets rewritten in C.)

Q12. Does project X already compile?
""""""""""""""""""""""""""""""""""""
No, unless project X has a tiny codebase with very few dependencies. The
culprit is that only a very small fraction of the libc is implemented.

Q13. Does profiling work?
"""""""""""""""""""""""""
No, `gcc -pg' etc. don't work, because the xtiny libc lacks the
instrumentation for profile stack trace collection.

Q14. Does debugging work?
"""""""""""""""""""""""""
Yes, if you specify the gcc -g flag for both compiling and linking. Of
course, the generated binary will be much larger because of the extra debug
info (and the linker script is not used either):

Without debugging:

  $ ./xtiny gcc examples/hellot.c
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 200 Oct 20 03:15 a.out

With debugging (-O0 is also recommended):

  $ ./xtiny gcc -g examples/hellot.c
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 3287 Oct 20 03:15 a.out

If you compile for debugging (-g), and you strip the objects later, then
they will become larger (252 bytes instead of 200 bytes in the example):

  $ ./xtiny gcc -g -c examples/hellot.c
  $ ./xtiny gcc hellot.o
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 252 Oct 20 03:15 a.out

Q15. Can I use the `strip' command to strip unneeded symbols?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
No, `strip' and most other ELF tools don't understand the non-debugging
output of xtiny, because the section table is missing.

You can use `sstrip' though, but it's unnecessary.

Fortunately, `xtiny gcc' already generates a small enough output by default,
containing only the necessary sections, so stripping wouldn't make it any
smaller.

Q16. Can I write multithreaded programs?
""""""""""""""""""""""""""""""""""""""""
Not easily and not out-of-the-box. First you have to fix errno, which is now
a global variable, and you should make it thread-local instead. Then you
have to write a threading library (possibly based on clone(2)).

Q17. Can I use third-party libraries?
"""""""""""""""""""""""""""""""""""""
pts-xtiny doesn't support dynamic libraries (.so files). It supports static
libraries (.a files). You can use such libraries the usual way: you have to
specify the -L... flag with the directory which contains the .a files, and
then -lfoo for libfoo.a in that directory.

Please note that there aren't any third-party libraries available yet the
author of pts-xtiny knows of.

Q18. Is pts-xtiny better than TCC (Tiny C Compiler)?
""""""""""""""""""""""""""""""""""""""""""""""""""""
It's hard to compare, because they have different goals. TCC
(http://bellard.org/tcc/) is a tiny compiler (in terms of file size) which
generates unoptimized, thus a bit bloated executables. pts-xtiny uses a
regular, optimizing compiler (GCC or Clang) and contains library functions
optimized for size, so it generates tiny executables.

If you are interested in a self-contained version of TCC (statically linked,
also containing the .a files), get pts-tcc from
http://ptspts.blogspot.com/2009/11/tiny-self-contained-c-compiler-using.html
.

Q19. I need a libc with many more features. How do I benefit from pts-xtiny?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
You can use the linker script xtiny.scr from pts-xtiny. Just append
`-Wl,-T,.../xtiny.scr' to your `gcc' command-line using another libc (such
as dietlibc or uClibc), and benefit from some extra stripping of unneeded
symbols.

If your code contains constructors or destructors (i.e. code which runs
before main or after exit(2)), xtiny.scr may not work for you though, and
you'll get a linker error.

Q20. Are executables created with pts-xtiny very fast?
""""""""""""""""""""""""""""""""""""""""""""""""""""""
Not particularly, because they are optimized for size (`gcc -Os') instead of
speed, and functions in the xtiny libc are also optimized for size (e.g.
memset is just a `rep stosb', quite slow compared to SIMD approaches (e.g.
MMX, SSE) on some CPUs).

If you need fast execution, you should give up on tiny executable sizes, and
you should try uClibc (or even EGLIBC or glibc) instead. With dietlibc, many
library functions are not optimized for speed.

__END__
