%include "types.asm"
%define SCREEN_PHYSADDR		0xb800
%define STAGE1_PHYSADDR		0x7c00
%define STAGE2_PHYSADDR		STAGE1_PHYSADDR + 0x0200
%define BOOTLOADER_STACK	0x7bfe
%define MEMMAP_PHYSADDR		0x0500

section .text

[bits 16]
stage1:

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BOOTLOADER_STACK

; read 32 sectors of awesome^W stage2
	mov ah, 2
	mov al, 32
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, 0
	mov bx, STAGE2_PHYSADDR
	int 0x13

; memory mapping
	xor ebx, ebx
	sub sp, 2
memmaploop:
	add sp, 2
memmaploop2:
	mov di, MEMMAP_PHYSADDR
	mov edx, 0x534d4150
	mov eax, 0xe820
	mov ecx, 24
	clc
	int 0x15
	pushf
	add di, 24
	test ebx, ebx
	jnz memmaploop
	popf
	jnc memmaploop2

; A20
	in al, 0x92
	or al, 2
	out 0x92, al

; protected mode
	cli
	lgdt [gdtr]
	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp 0x1:stage2

; GDT data
gdtr:
nulldesc:
	dw gdtr
	times	4	db	0
gdt1:						; code segment
istruc GDT_entry
	.limit_l	dw	0x0000
	.base_lw	dw	0xFFFF
	.base_hwl	db	0xFF
	.access_byte	db	10011010b
	.flags_limit	db	11001111b
	.base_hwh	db	0xFF
iend
gdt2:						; data segment
istruc GDT_entry
	.limit_l	dw	0x0000
	.base_lw	dw	0xFFFF
	.base_hwl	db	0xFF
	.access_byte	db	10010010b
	.flags_limit	db	11001111b
	.base_hwh	db	0xFF
iend

times 510-($-stage1) db 0
dw 0x55AA

stage2:
