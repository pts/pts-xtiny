/* by pts@fazekas.hu at Mon Oct 20 00:31:44 CEST 2014 */
/*.arch i386*/
.section .text.__xtiny_startexit
.globl _start
.type _start, @function
_start:
/* This is hand-optimized assembly code of 43 bytes, `gcc-4.6 -Os'
 * generates 53 bytes.
 *
 * The addresses in the assembly dump below are relative to _start.
 */
.byte 0x8D, 0x5C, 0x24, 0x04
/* ^^^ 00000000  8D5C2404          lea ebx,[esp+0x4] */
.byte 0x89, 0xDA
/* ^^^ 00000004  89DA              mov edx,ebx */
.byte 0x31, 0xC0
/* ^^^ 00000006  31C0              xor eax,eax */
.byte 0x83, 0xC3, 0x04
/* ^^^ 00000008  83C304            add ebx,byte +0x4 */
.byte 0x3B, 0x03
/* ^^^ 0000000B  3B03              cmp eax,[ebx] */
.byte 0x75, 0xF9
/* ^^^ 0000000D  75F9              jnz 0x8 */
.byte 0x8D, 0x4B, 0x04
/* ^^^ 0000000F  8D4B04            lea ecx,[ebx+0x4] */
.byte 0x29, 0xD3
/* ^^^ 00000012  29D3              sub ebx,edx */
.byte 0xC1, 0xEB, 0x02
/* ^^^ 00000014  C1EB02            shr ebx,0x2 */
.byte 0x89, 0x0D
.long __xtiny_environ
/* ^^^ 00000017  890D????????      mov [__xtiny_environ],ecx */
.byte 0x51
/* ^^^ 0000001D  51                push ecx */
.byte 0x52
/* ^^^ 0000001E  52                push edx */
.byte 0x53
/* ^^^ 0000001F  53                push ebx */

#ifdef DO_INIT_ARRAY
/* by pts@fazekas.hu at Sun Jan 29 10:16:14 CET 2017
 * http://ptspts.blogspot.com/2014/01/how-to-run-custom-code-before-and-after.html
 */
/* This works only if .init_array is not empty. */
mov $__init_array_start, %eax
.Linitarycall:
push %eax
call *(%eax)
pop %eax
add $4, %eax  /* 3 bytes only. */
cmp $__init_array_end, %eax
jne .Linitarycall
#endif

call main
/* ^^^ 000000??  E8????????        call main */
/* Now eax has the exit code. */
/* Execution continues in __xtiny_exit.s. */
.size _start, .-_start
/* Make __xtiny_exit undefined here, pull __xtiny_exit.s. */
.reloc 0, R_386_NONE, __xtiny_exit
