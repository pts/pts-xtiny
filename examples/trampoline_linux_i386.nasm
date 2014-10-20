; by pts@fazekas.hu at Sun Oct 19 12:51:22 CEST 2014
;
; Compile with:
;
;   $ nasm -f bin -o cgin.trampoline trampoline_linux_i386.nasm && chmod +x cgin.trampoline
;
; Based on the output of:
;
;   $ gcc -S -masm=intel -m32 -W -Wall -Os -Wl,-z,norelro -Wl,--build-id=none -fno-ident -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-stack-protector -fomit-frame-pointer -mpreferred-stack-boundary=2 -falign-functions=1 -falign-jumps=1 -falign-loops=1 trampoline_linux_i386.c && cat trampoline_linux_i386.s
;
bits 32
  
; Based on: http://www.muppetlabs.com/~breadbox/software/tiny/teensy.html
org 0x08048000
ehdr:                              ; Elf32_Ehdr
db 0x7F, "ELF", 1, 1, 1, 0         ;   e_ident
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
dw 0                               ;   e_shentsize
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

; This is based on trampoline_linux_i386.c
_start:
	push	ebx
	lea	edx, [esp+8]
.L2:
	cmp	DWORD [edx], 0
	lea	edx, [edx+4]
	jne	.L2
	xor	ecx, ecx
	mov	eax, 206
	mov	ebx, ecx
	int 0x80
	mov	eax, DWORD [esp+8]
	mov	ecx, eax
	jmp	.L4
.L6:
	sub	ebx, 46
	cmp	bl, 1
	cmovbe	ecx, eax
	inc	eax
.L4:
	mov	bl, BYTE [eax]
	test	bl, bl
	jne	.L6
	mov	BYTE [ecx], 0
	mov	eax, 11
	mov	ebx, DWORD [esp+8]
	lea	ecx, [esp+8]
	int 0x80
	mov	eax, 1
	mov	ebx, 120
	int 0x80

filesize equ $ - $$
