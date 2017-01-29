/* by pts@fazekas.hu at Sun Jan 29 20:56:18 CET 2017 */
.section .text.__xtiny_startexit
.globl __xtiny_exit_with_fini
.type __xtiny_exit_with_fini, @function
__xtiny_exit_with_fini:  /* Exit code passed in eax (regparm(1)). */
.globl __xtiny_exit
.type __xtiny_exit, @function
__xtiny_exit:  /* Exit code passed in eax (regparm(1)). */
.byte 0x93
/* ^^^ 000000??  93                xchg eax,ebx */
.byte 0x31, 0xC0
/* ^^^ 000000??  31C0              xor eax,eax */
.byte 0x40
/* ^^^ 000000??  40                inc eax */
.byte 0xCD, 0x80
/* ^^^ 000000??  CD80              int 0x80 */
.size __xtiny_exit_with_fini, .-__xtiny_exit_with_fini
.size __xtiny_exit, .-__xtiny_exit
