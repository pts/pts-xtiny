README for pts-xtiny
^^^^^^^^^^^^^^^^^^^^
pts-xtiny is a collection of tools and a small and incomplete libc for
creating tiny (200-byte), statically linked ELF executables on Linux i386.

pts-xtiny was inspired by the tiny ELF assembly header in ``A Whirlwind
Tutorial on Creating Really Teensy ELF Executables for Linux''
(http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html). pts-xtiny
achieves almost the same, tiny file size as indicated there, but without the
programmer having to write any assembly code. The ELF file generation is
done using a GNU ld command-line flags and truncation of the generated
executable (similarly to sstrip). pts-xtiny has a linker script, but it's
not used by default.

FAQ
~~~

Q0. How do I install pts-xtiny?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
You need a Linux i386 or amd64 system.

First install Python (you can also use
https://github.com/pts/staticpython/raw/master/release/python2.7-static).

Then download xtiny:

  $ git clone https://github.com/pts/pts-xtiny
  $ cd pts-xtiny
  $ ./xtiny

You can put the pts-xtiny directory to your $PATH, so that you can type just
`xtiny' instead of `./xtiny'.

Install a C compiler: GCC or Clang, preferably gcc.
You can install the compiler from distribution package
(e.g. `sudo apt-get install gcc' on Debian-ish systems), or you can download
and use pts-clang (see Q35) without installation.

Once everything is set up, compile with `xtiny gcc -Os ...' instead of `gcc
...'. For example, this is how to compile the hello-world:

  $ ./xtiny gcc -Os examples/hello.c
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 234 Feb 12 17:38 a.out

You can use most of the gcc flags (e.g. -W -Wall -Wextra -Werror).

Q1. How large is a typical executable?
""""""""""""""""""""""""""""""""""""""
A typical Hello, World program is only 200 bytes:

  $ ./xtiny gcc examples/hellot.c
  $ ./a.out
  Hello, World!
  $ ls -l a.out
  -rwxr-xr-x 1 pts pts 200 Oct 20 03:15 a.out

The hand-optimized assembly version, compiled from examples/hellowr.nasm, is
138 bytes long, 62 bytes shorter.

After the `gcc' in the command above almost all gcc flags are supported (and
will be passed to gcc). Examples of supported flags: -c, -S, -E, -M, -MM,
-W..., -f..., -m... .

If you forget to specify the `-Os' flag to make the compiler optimize for
file size, the `xtiny' wrapper will add it for you. Unless you specify another
`-O...' flag, the wrapper also adds some other relevant gcc flags to make
the output file even smaller.

Q1B. I have a large program. How much can pts-xtiny reduce the executable size?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
The maximum reduction from `gcc -m32' to `xtiny gcc' is about 5400 bytes.
The maximum reduction from `xstatic gcc' to `xtiny gcc' is about 4100 bytes.
These numbers are based on compiling examples/hellowr.c with gcc-4.8.4.

So if your original executable is larger than 100 kB, then pts-xtiny probably
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

pts-xtiny includes a linker (ld.xtiny) and an assembler (as.xtiny), and it
doesn't use the GNU Binutils installed to your system. Thus your Binutils
can be very old (and incompatible with pts-xtiny), pts-xtiny will still work
consistently.

Q3. Which architectures are supported?
""""""""""""""""""""""""""""""""""""""
i386 (x86, 32-bit) only. The generated binaries will run on amd64 (x86_64)
Linux systems as well.

There is no need to specify the `-m32' flag for gcc, `xtiny' specifies it by
default.

Q4. Which operating systems are supported?
""""""""""""""""""""""""""""""""""""""""""
Linux only, and also FreeBSD in Linux emulation mode.

pts-xtiny is not able to produce .exe files (for Windows) or macOS
executables.

Q5. Do the executables link against glibc, eglibc, uClibc, dietlibc, musl?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
No, they link statically against the pts-xtiny libc only, which is included
in the pts-xtiny distribution. At runtime no external files are necessary to
run the executables.

Q6. The pts-xtiny libc is very small, will you add function X?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
If X is a Linux system call, please send me a patch, and I'll add it.
Otherwise let's discuss first to avoid the extra work.

Your contributions are welcome and appreciated.

See also Q31 for readily available alternatives such as pts-xstatic.

Q7. Which programming languages are supported?
""""""""""""""""""""""""""""""""""""""""""""""
Currently C and i386 assembly (as accepted by the GNU assembler) are
supported.

C++ may get added in the future, but there will be no exception handling, no
STL, and libstdc++ will not be available. For C++ pts-xtiny is missing
malloc (for `operator new') and C++-style constructors and destructors
(__dso_handle and __cxa_atexit).

Q8. Can I use existing, precompiled libraries?
""""""""""""""""""""""""""""""""""""""""""""""
Most probably they won't work, because most of them need lots of functions
from the libc (which the pts-xtiny libc doesn't have), and when they were
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
* the GNU linker: the ld tool in GNU Binutils
  or the GNU gold linker: the gold tool in GNU Binutils
* Python >=2.4 (This may get removed in the future when the `xtiny' wrapper
  script gets rewritten in C.)

Q12. Does project X already compile?
""""""""""""""""""""""""""""""""""""
No, unless project X has a tiny codebase with very few dependencies. The
culprit is that only a very small fraction of the libc is implemented.

Q13. Does profiling work?
"""""""""""""""""""""""""
No, `gcc -pg' etc. don't work, because the pts-xtiny libc lacks the
instrumentation for profile stack trace collection.

Q14. Does debugging work?
"""""""""""""""""""""""""
Yes, if you specify the gcc -g flag for both compiling and linking. Of
course, the generated binary will be much larger because of the extra debug
info.

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
output of pts-xtiny, because the section table is missing.

You can use `sstrip' though, but it's unnecessary.

Fortunately, `xtiny gcc' already generates a small enough output by default,
containing only the necessary sections, so stripping wouldn't make it any
smaller.

Q16. Can I write multithreaded programs with pts-xtiny?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""
pts-xtiny can't help you if you need to write multithreaded programs.

These have to be changed:

* Fix errno, which is now a global variable, and make it thread-local instead.
* Write a threading library (possibly based on clone(2)).
* Fix malloc and other library functions which don't do any locking.

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
You can copy some of the command-line flags `xtiny' passes to gcc and ld.

You can use the linker script xtiny.scr from pts-xtiny. Just append
`-Wl,-T,.../xtiny.scr' to your `gcc' command-line using another libc (such
as dietlibc, musl or uClibc), and benefit from some extra stripping of
unneeded symbols.

Q20. Are executables created with pts-xtiny very fast?
""""""""""""""""""""""""""""""""""""""""""""""""""""""
Not particularly, because they are optimized for size (`gcc -Os') instead of
speed, and functions in the pts-xtiny libc are also optimized for size (e.g.
memset is just a `rep stosb', quite slow compared to SIMD approaches (e.g.
MMX, SSE) on some CPUs).

If you need fast execution, you should give up on tiny executable sizes, and
you should try uClibc (or even EGLIBC or glibc) instead. With dietlibc, many
library functions are not optimized for speed.

Q21. Is there a non-trivial example program using pts-xtiny?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
See examples/addrnd.c, which implements a Lagged Fibonacci generator,
writing random bytes to its stdout. Compiled executable size is 436 bytes.

See examples/multi_trampoline.c, which prepares the Linux process state
(chdir(), chroot(), umask(), setreuid(), environ) and then runs another
program using execve(). Compiled executable size is between 400 and 700
bytes, depending on the command-line flags.

See https://github.com/pts/ssh_connect_fast , which implements a trampoline
wrapper around ssh(1), changing environment variables and commend-line flags
for faster connection setup. Compiled executable size is 1693 bytes.

See https://github.com/pts/pts-zcat/blob/master/compile_xtiny.sh , which
implements a .gz, .zip and Flate decompression filter (stdin-to-stdout).
Compiled executable size is 4600 bytes.

See https://github.com/pts/pts-line-bisect/blob/master/pts_lbsearch.c ,
which implements binary search for sorted on-disk text files, with a smart
and optimized input buffering. Compiled executable size is 6063 bytes.

See https://github.com/pts/tiny-ssh-keygen-ed25519 , which implements
ssh-keygen (SSH keypair generator) for ed25519 keys. Compiled executable
size is 7568 bytes.

See https://github.com/pts/pts-clang-xstatic/blob/master/xstatic.c (and also
other .c files in this repository), which implements a trampoline wrapper
for running gcc with specific flags for linking against uClibc. Compiled
executable size is 9052 bytes.

See https://github.com/pts/pts-tiny-7z-sfx , which implements a .7z archive
extractor and SFX. It doesn't use pts-xtiny directly, but features of
pts-xtiny are inlined to its c-minidiet.sh and minidiet/* files. Compiled
executable size is 24220 bytes; when compressed with upx, it's 16552 bytes.

Q22. Does pts-xtiny support -Wl,--gc-sections?
""""""""""""""""""""""""""""""""""""""""""""""
Yes, and you can use -mxtiny-gcs as a shorthand for
`-ffunction-sections -fdata-sections -Wl,--gc-sections'.

These flags together make sure than unused global variables and functions
are removed from the output executable, further saving file size.

Q23. Are executables produced by pts-xtiny as secure as default binaries by
gcc?
""""
No, executables produced by pts-xtiny are less secure, because pts-xtiny
specified `gcc -fno-stack-protector' and it has rwx pages (including
executable stack pages).

Q24. Can the output of pts-xtiny be further compressed?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""
upx (http://upx.github.io/) works on the output executables of pts-xtiny, but
you need a large executable (typically >10 kB, probably won'y work for <5 kB)
to see a size improvement. See examples/compressible.c for an example.

Q25. How to get compiler warnings for <xtiny.h>?
""""""""""""""""""""""""""""""""""""""""""""""""
Since xtiny.h is in an `-isystem' directory, to get warnings for it,
compile it with: `-Werror -Wsystem-headers', in addition to the usual
`-W -Wall -Wextra'.

Q26. How can I check in a .c file if it's compiling with pts-xtiny?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Use `#ifdef __XTINY__'. As usual, `#if __XTINY__' also works.

Q27. Does pts-xtiny use a linker script (ld -T)?
""""""""""""""""""""""""""""""""""""""""""""""""
Not (anymore) by default. Enable it with `xtiny ... -mxtiny-linker-script'
if you need it.

Known limitations of `-T xtiny.scr' enabled by -mxtiny-linker-script:

* xtiny.scr strips debug symbols, so don't use it with `xtiny gcc -g'.
* xtiny.scr has OUTPUT_FORMAT(binary), which makes `ld' ignore
  `--gc-sections'. There is a workaround for that in wrap_linker
  (`ld -r'), but that removes .init_array when it shouldn't. This can be
  surprising to the user, so we just disable it.
* xtiny.scr doesn't produce smaller output than the alternative: `ld -N'
  and sstrip_elf_executable. (xtiny.scr used to be better until the
  alternative was improved in xtiny.)
* GNU gold (the alternative linker to GNU ld) doesn't support linker
  scripts (-T), and GNU gold may be used in some Clang configurations (e.g.
  pts-clang) by default.

In general, the output executables with or without -mxtiny-linker-script
should have the same size but not exactly the same content (because sections
are ordered differently by the linker). If you experience a discrepancy
other than those below, please report an issue.

Q28. Does pts-xtiny support initializers like __attribute__((constructor)) and
__attribute__((destructor))?
""""""""""""""""""""""""""""
Yes. Even better: if these features are not used, they don't add overhead to
the executable. (May other libcs get this wrong, they add overhead.)

Note if you are using -mxtiny-ld=... with an old GNU ld. ld in GNU Binutils
2.20.1 is buggy: it removes constructors and destructors even without
--gc-sections. If this affects you, upgrade your GNU Binutils to 2.24 (which
is known to work).

(Please note that constructors and destructors are silently ignored with
-mno-xtiny-wrap-linker.)

Q29. Does pts-xtiny support atexit(3) or on_exit(3)?
""""""""""""""""""""""""""""""""""""""""""""""""""""
pts-xtiny supports atexit(3) with only 1 function registered (ATEXIT_MAX ==
1), and it doesn't support on_exit(3).

However, you can register multiple functions with
__attribute__((destructor)), and the generated code is also shorter, so use:

  __attribute__((destructor)) static void mydtor1(void) { ... }
  __attribute__((destructor)) static void mydtor2(void) { ... }

... instead of

  static void mydtor(void) { ... }
  ...
  atexit(mydtor)

Q30. Does pts-xtiny support linking with GNU gold?
""""""""""""""""""""""""""""""""""""""""""""""""""
Yes, just specify `xtiny ... -mxtiny-ld=gold' (or the full path to the gold
executable).

-mxtiny-gcs (and -Wl,--gc-sections) works as expected with GNU gold.

The output file should be of the same size with our without GNU gold, and it
should be byte-by-byte identical. If you experience a discrepancy, please
report an issue.

Q31. I need many more libc functions. What should I do?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""
If you need these functions urgently, pts-xtiny probably won't help you. You
can use uClibc, dietlibc or musl as alternatives for Linux i386.

For your convenience (easy installation and it works with any compiler
already installed), I recommend you pts-xstatic
(https://raw.githubusercontent.com/pts/pts-clang-xstatic/master/README.pts-xstatic.txt),
which uses (and contains a copy of) uClibc. It also supports C++98, and
contains a copy of libstdc++. To use it, install it, and then just replace
`xtiny gcc' with `xstatic gcc', and hundrends of libc functions become
available.

Please note that other libcs (including pts-xstatic) have more overhead than
pts-xtiny, so your executables will be a few kilobytes larger.

Q32. Can I define my own entry point (_start function)?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""
Yes, it works with or without `xtiny ... -nostartfiles', with 0 overhead.

You can specify -nostartfiles, but it's generally not needed, because even
if you don't specify it, your definition of _start will take precedence over
the definition in the pts-xtiny libc.

Q33. How do I get rid of the pts-xtiny libc in my executable?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Specify `xtiny ... -nodefaultlibs' or `xtiny ... -nostdlib', these GCC flags
work as documented.

Please note that some functions (such as system call read(2) and library
function `puts') defined in xtiny.h will still work with -nodefaultlibs and
-nostdlib, because they are defined `static inline'.

Q34. How do I use a custom GNU ld binary with pts-xtiny?
""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Specify `xtiny ... -mxtiny-ld=PATH_TO_LD'.

If you specify `xtiny ... -mxtiny-ld=.xtiny', pts-xtiny will use the bundled
linkr (ld.xtiny). This is the default behavior, because the system may have
a very old GNU ld, which prevents some features (such as
__attribue__((constructor))) from working.

If you specify `xtiny ... -mxtiny-ld=.compiler', pts-xtiny will use whatever
the specificed compiler would use by default. This is not recommended,
because he system may have a very old GNU ld (see above).

Q35. Does pts-xtiny support malloc, free, realloc and calloc?
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
To get them (exept for realloc), specify `xtiny ... -mxtiny-forward-malloc'.

Use realloc_grow instead of realloc. You'll need to specify the old block
size as well.

Please note that free() is a no-op, and memory once allocated and free()ed
will never be reused for subsequent allocations.

You can use about 1.2 GB of heap by default. To get more, specify something
like `xtiny ... -mxtiny-forward-malloc=2100000000'. Anything much larger than
2.1 GB won't work. Anything larger than 0x7fffffff won't even compile.

Alternatively, if you don't want to specify -mxtiny-forward-alloc, you can
add this to your .c file:

  #ifndef __XTINY_FORWARD_MALLOC__
  #define __XTINY_FORWARD_MALLOC__ 1200000000  /* Almost 1.2 GB. */
  extern char __forward_malloc_heap[];
  char *__forward_malloc_heap_end =
      __forward_malloc_heap + __XTINY_FORWARD_MALLOC__;
  #endif
  #include <xtiny.h>

Q36. How do I use pts-xtiny with pts-clang?
"""""""""""""""""""""""""""""""""""""""""""
This may not work, e.g. compilation will fail or it will create buggy
code.

Download pts-xtiny first, and put it to your $PATH.

Example download of pts-clang:

  $ rm -f pts-clang-latest.sfx.7z
  $ wget http://pts.50.hu/files/pts-clang/pts-clang-latest.sfx.7z
  $ chmod +x pts-clang-latest.sfx.7z
  $ ./pts-clang-latest.sfx.7z -y  # Creates the pts-clang directory.

Use it as: `xtiny .../pts-clang/bin/clang ...'.

You can also put pts-clang/bin/clang to (the beginning of) your $PATH, and
then use it as: `xtiny clang ...'.

Technical notes
~~~~~~~~~~~~~~~
Useful links
""""""""""""
* https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
* elfkickers (includes sstrip):
  http://www.muppetlabs.com/~breadbox/software/elfkickers.htmls
* https://refspecs.linuxbase.org/LSB_3.0.0/LSB-PDA/LSB-PDA/specialsections.html

Executable stack info
"""""""""""""""""""""
gcc generates a `.section        .note.GNU-stack,"",@progbits' line, which
generates a PT_GNU_STACK program header, which we don't need. It's possible
to patch the .s file to remove this line or to remove or rename the section
.note.GNU-stack in the .o file. pts-xtiny is doing the latter, renaming the
section to .note.xty-stack .

Alignment by ld
"""""""""""""""
ld takes the maximum alignment in each .o file (per section?), thus if an
.s file has `.p2align 7,,127' near the beginning to align to 128 bytes, then
each .o file will be aligned to 128 bytes.

ld -T tiny.scr doesn't respect such alignments, it will align the first
section to 4-byte boundary (file offset 0x54).

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

About symbol lookup order in .o and .a files
""""""""""""""""""""""""""""""""""""""""""""
* See http://eli.thegreenplace.net/2013/07/09/library-order-in-static-linking
* ar remembers the order the archive is created:
  rm -f foobar2.a; ar cr foobar2.a foo.o bar2.o
  rm -f bar2foo.a; ar cr bar2foo.a bar2.o foo.o
  nm -s foobar2.a
  nm -s bar2foo.a
  ar t foobar2.a
  ar t bar2foo.a
* GNU ar runs (a built-in) ranlib by default if .o files are added
* libA.a will look for symbol definitions in libA.a only; and later ld will
  look in subsequent .a files in the command-line (and not earlier ones) to
  resolve symbols undefined after libA.a
* for circular dependencies, specify `-\( libA.a libB.a -\)' or (only if 1
  layer of dependency), `libA.a libB.a libA.a'.

How to print the active linker script
"""""""""""""""""""""""""""""""""""""
ld --verbose

TODOs
~~~~~
* TODO: Anomaly: Diagnose file size anomalies in TODO in
  https://github.com/pts/pts-clang-xstatic/blob/master/mktrampolines-minidiet
* TODO: Provide a non-inline version of puts in lib__xtiny.a.
* TODO: Why are there \0s at the end of tgen? Can't we move them to bss? Add .py code to truncate.
* TODO: Why is the file large with (Q14): `xtiny gcc -g' + `sstrip'?
* TODO: Add lib directory for possible additional user libraries, add it by default as -L
* TODO: Enumerate sections:
  https://refspecs.linuxfoundation.org/LSB_2.1.0/LSB-Core-generic/LSB-Core-generic/specialsections.html
* TODO: How to use CONSTRUCTORS etc. in the linker script?
* TODO: Add support for debugging with gdb. -g shouldn't remove symbols
  etc., maybe shouldn't use the linker script.
* TODO: Convert the xtiny tool from Python to .c.
* TODO: Add C++ support.
* TODO: Make it work with clang: now segfault and large binary for
  pts-clang, and bad string constants for regular clang.
  clang-3.4.bin: error: unknown argument: '-mpreferred-stack-boundary=2'
  clang-3.4.bin: error: unknown argument: '-falign-jumps=1'
  clang-3.4.bin: error: unknown argument: '-falign-loops=1'
  Linking with the linker script doesn't work, gets linked regularly, and the
  result is a segfault.
  __builtin_strlen acts really strangely.
  Also pts-clang complains about -B flag with default xtiny.
* TODO: Anomaly: Why is pts_lbsearch.xtiny (in pts-line-bisect) 6888 bytes when
  compiled with clang-4.4, and only 6063 bytes when compiled with gcc-4.8?
* TODO: pts-clang bug: no include paths in -E -.
* TODO: bad string constants for regular clang packages.
* TODO: clang-3.9: warning: optimization flag '-falign-functions=1' is not supported
* TODO: __builtin_strlen acts really strangely -- does it still with pts-clang and regular clang?
* TODO: Use glibc's smart __extern_always_inline instead?
* TODO: Does __builtin_memcpy get hardwired with gcc -O3?
* TODO: Add proper C++ `const char*' overloads for strstr etc, like
  glibc string.h
* TODO: Why is gcc-4.4 tinyt.c two.c only 424 bytes, but with gcc-4.6 it's
  436 bytes?
* TODO: Import syscalls from libsys (syscall-only libc, 1999). There is
  socketcall, but no individual calls. What about constness etc.
  Maybe musl, dietlibc, uClibc?
* TODO: doc: What are the similar projects to xtiny?
* TODO: Why is `xtiny gcc -g' ... `sstrip' output so large? (See above.)
* TODO: Avoid the padding of _start.s with 0x90 etc. for __xtiny_environ
  (which is aligned to 4 bytes with 0x66, 0x90 for -mno-xtiny-linker-script).
* TODO: Copy some gcc flags from musl: -nostdinc -ffreestanding -fexcess-precision=standard -frounding-math -march=i486 -mtune=generic
* TODO: Document some gcc warning flags from musl (some of them need a new gcc): -Werror=missing-declarations -Werror=implicit-function-declaration -Werror=implicit-int -Werror=pointer-sign -Werror=pointer-arith
* TODO: use malloc_lite (and calloc_lite) if no free
* TODO: Anomaly: Why does examples/addrnd.c compile to 512 bytes with g++-4.8,
  rather than 441 bytes with gcc-4.8? What's the difference in code generation?

__END__
