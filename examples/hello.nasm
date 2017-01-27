; by pts@fazekas.hu at Mon Oct 20 15:38:20 CEST 2014
;
; Compile with:
;
;   $ nasm -f bin -o hello hello.nasm && chmod +x hello
;
; The output will be 138 bytes.
;

bits 32

; Based on: http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html
org 0x08048000
ehdr:                              ; Elf32_Ehdr
db 0x7F, "ELF", 1, 1, 1            ;   e_ident
db 3                               ;   0=SYSV 3=GNU/Linux
dd 0, 0
dw 2                               ;   e_type
dw 3                               ;   e_machine
dd 1                               ;   e_version
dd _start                          ;   e_entry
dd phdr - $$                       ;   e_phoff
dd 0                               ;   e_shoff
dd 0                               ;   e_flags
dw ehdrsize                        ;   e_ehsize
dw phdrsize                        ;   e_phentsize
dw 1                               ;   e_phnum
dw 40                              ;   e_shentsize
dw 0                               ;   e_shnum
dw 0                               ;   e_shstrndx
ehdrsize equ $ - ehdr
phdr:                              ; Elf32_Phdr
dd 1                               ;   p_type
dd 0                               ;   p_offset
dd $$                              ;   p_vaddr
dd $$                              ;   p_paddr
dd filesize                        ;   p_filesz
dd filesize                        ;   p_memsz
dd 7                               ;   p_flags  ; (1=executable 2=writable 4=readable)
dd 0x1000                          ;   p_align
phdrsize      equ     $ - phdr

; This is hand-optimized assembly for a loop which calls write(2) until
; all the output is printend.
_start:
mov edx, msg_size
mov ecx, msg
.again:
mov eax, 4  ; __NR_write == 4.
xor ebx, ebx
inc ebx  ; STDOUT_FILENO == 1.
int 0x80
add ecx, eax
neg eax
add eax, edx
jz .done  ; Finished writing.
cmp eax, edx
xchg eax, edx
jle .again  ; Error writing.
.done:
xor eax, eax
mov ebx, eax
inc eax  ; __NR_exit == 1.
int 0x80
msg: db 'Hello, World!', 10
msg_size equ $ - msg

filesize equ $ - $$
