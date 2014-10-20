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
gcc-4.4 and later. Probably it would be possible to make it work with
gcc-4.3, gcc-4.2 and gcc-4.1 if there is interest.

You have to install the compiler separately. You can install it from package
(e.g. `sudo apt-get install gcc' on Debian-ish systems).

clang doesn't work yet, there are some strange bugs, for example string
constants (char[]) are not propagated properly.

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
run them.

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

__END__
