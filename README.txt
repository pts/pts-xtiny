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

Q1B. I have a large program. How much can xtiny reduce the executable size?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
The maximum reduction from `gcc -m32' to `xtiny gcc' is about 5400 bytes.
The maximum reduction from `xstatic gcc' to `xtiny gcc' is about 4100 bytes.
These numbers are based on compiling examples/hellowr.c with gcc-4.8.4.

So if your original executable is larger than 100 kB, then xtiny probably
won't make a big difference.

You may want to try reduce your library dependencies, refactor your code,
and compress the executable (with `upx --brute', https://upx.github.io/)
first.

See also more ideas here:
http://ptspts.blogspot.ch/2013/12/how-to-make-smaller-c-and-c-binaries.html

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

xtiny is not able to produce .exe files (for Windows) or macOS executables.

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
true for pts-xtiny. So even if it compiles, it will probably segfault.

Q9. Can I build and use shared libraries (.so files)?
"""""""""""""""""""""""""""""""""""""""""""""""""""""
The design of pts-xtiny fundamentally doesn't support .so files, so that will
never work.

Q10. What gcc flags are recommended?
""""""""""""""""""""""""""""""""""""
You don't have to specify -m32, -Os, -static or -s, these are automatically
specified by the `xtiny' wrapper script. Unless you specify another
`-O...' flag, the wrapper also adds some other relevant gcc flags to make
the output file even smaller.

It's recommended that you specify some warning flags, like these: -W -Wall
-Wextra -Werror -Wsystem-headers.

You may also want to specify -ansi or -ansi -pedantic, but that usually
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

Q21. Is there a non-trivial example program using pts-xtiny?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
See examples/addrnd.c, which implements a Lagged Fibonacci generator,
writing random bytes to its stdout.

Q22. Does xtiny support -Wl,--gc-sections?
""""""""""""""""""""""""""""""""""""""""""
Yes, and you can use -mxtiny-gcs as a shorthand for
`-ffunction-sections -fdata-sections -Wl,--gc-sections'.

These flags together make sure than unused global variables and functions
are removed from the output executable, further saving file size.

Q23. Are executables produced by xtiny as secure as default binaries by gcc?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
No, executables produced by xtiny are less secure, because xtiny specified
`gcc -fno-stack-protector' and it has rwx pages (including stack pages).

Q24. Can the output of xtiny be further compressed?
"""""""""""""""""""""""""""""""""""""""""""""""""""
Probably for large executables (>10 kB) upx (http://upx.github.io/) is able to
reduce the file size even further. Give it a try. But for small executables
(<6 kB), there is probably no improvement.

Q25. How to get compiler warnings for xtiny.h?
""""""""""""""""""""""""""""""""""""""""""""""
Since xtiny.h is in an `-isystem' directory, to get warnings for it,
compile it with: `-Werror -Wsystem-headers', in addition to the usual
`-W -Wall -Wextra'.

Q26. How can I check in a .c file if it's compiling with xtiny?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Use `#ifdef __XTINY__'. As usual, `#if __XTINY__' also works.

Technical notes
~~~~~~~~~~~~~~~
Useful links
""""""""""""
* elfkickers (includes sstrip):
  http://www.muppetlabs.com/~breadbox/software/elfkickers.htmls

How is xtiny.scr different from sstrip
""""""""""""""""""""""""""""""""""""""
What was compared:

* xtiny.scr: xtiny gcc ... -o sample
* sstrip: xtiny gcc -mno-xtiny-linker-script ... -o samplebs && sstrip samplebs
* gcc-4.8.4, GNU ld-2.24

Differences:

* The system ID at ELF offset 6 is 3 (GNU/Linux) for sample and 0 (SYSV)
  for samplebs.
* Symbols are ordered differently, e.g. the entry point (e_entry) is
  different. But the total size of the sections in the file is the same.
* (The smallest p_vaddr is the same: 0x08048000.)
* samplebs is 64 bytes larger than sample, because it contains 3 (instead of 1)
  program header entries.
  * Program headers hexdump:
    # Program header entry 0/1 in sample:
    01000000 p_type: LOAD
    00000000 p_offset
    00800408 p_vaddr
    00800408 p_paddr
    28170000 p_filesz
    B0170000 p_memsz
    07000000 p_flags: rwx
    00100000 p_align
    # Program header entry 0/3 in samplebs:
    01000000 p_type: LOAD
    00000000 p_offset
    00800408 p_vaddr
    00800408 p_paddr
    68170000 p_filesz
    68170000 p_memsz
    05000000 p_flags: r-x
    00100000 p_align
    # Program header entry 1/3 in samplebs:
    01000000 p_type: LOAD
    68170000 p_offset
    68A70408 p_vaddr
    68A70408 p_paddr
    00000000 p_filesz
    88000000 p_memsz
    06000000 p_flags: rw-
    00100000 p_align
    # Program header entry 2/3 in samplebs:
    51E57464 p_type: STACK
    00000000 p_offset
    00000000 p_vaddr
    00000000 p_paddr
    00000000 p_filesz
    00000000 p_memsz
    07000000 p_flags
    10000000 p_align: 2**4
  * The STACK entry can probably be omitted (can it?).
  * The two LOAD entries are adjacent and can be merged to a single rwx page.

TODOs
~~~~~
* TODO: Add tiny nasm, as, yasm "Hello, World\n" programs.
* TODO: Why are there 0s at the end of tgen? Can't we move them to bss?
* TODO: Why is the file large with (Q14): `xtiny gcc -g' + `sstrip'?
* TODO: Add a better sstrip (which removes STACK and merges LOAD) to xtiny,
  and use it by default with -mno-xtiny-linker-script. Also make it emit the
  correct system ID (for FreeBSD compatibility).
* TODO: gold + sstrip works. gold --gc-sections -r workaround doesn't work.
* TODO: Does upx work on larger files emitted by xstatic?
* TODO: Add -lgcc for long long division etc.
* TODO: Add lib directory for possible additional user libraries.
* TODO: Auto-detect GNU gold and use the built-in linker if available.
  Please note /usr/bin/ld vs /usr/bin/gold.
* TODO: Enumerate sections:
  https://refspecs.linuxfoundation.org/LSB_2.1.0/LSB-Core-generic/LSB-Core-generic/specialsections.html
* TODO: How to use CONSTRUCTORS etc. in the linker script?
* TODO: Add support for debugging with gdb. -g shouldn't remove symbols
  etc., maybe shouldn't use the linker script.
* TODO: Convert the xtiny tool from Python to .c.
* TODO: Add C++ support, possibly for many kinds of initializers.
* TODO: Make it work with clang: now segfault and large binary for
  pts-clang, and bad string constants for regular clang.
  clang-3.4.bin: error: unknown argument: '-mpreferred-stack-boundary=2'
  clang-3.4.bin: error: unknown argument: '-falign-jumps=1'
  clang-3.4.bin: error: unknown argument: '-falign-loops=1'
  Linking with the linker script doesn't work, gets linked regularly, and the result is a segfault.
  __builtin_strlen acts really strangely.
* TODO: pts-clang bug: no include paths in -E -.
* TODO: Use glibc's smart __extern_always_inline instead?
* TODO: Does __builtin_memcpy get hardwired with gcc -O3?
* TODO: Add proper C++ `const char*' overloads for strstr etc, like
  glibc string.h
* TODO: Why is gcc-4.4 tinyt.c two.c only 424 bytes, but with gcc-4.6 it's
  436 bytes?
* TODO: Import syscalls from libsys (syscall-only libc, 1999). There is
  socketcall, but no individual calls. What about constness etc.
  Maybe uclibc?
* TODO: doc: What are the similar projects?
* TODO: Why is `xtiny gcc -g' ... `strip' output so large?

__END__
