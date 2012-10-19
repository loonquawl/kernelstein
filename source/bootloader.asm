%include "types.asm"
%define SCREEN_SEGMENT		0xb800
%define STAGE1_PHYSADDR		0x7c00
%define STAGE2_PHYSADDR		0x7e00
%define BOOTLOADER_STACK	0x7bfe
%define MEMMAP_PHYSADDR		0x0500

org STAGE1_PHYSADDR

global _start
_start:

bits 16
stage1:

	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, BOOTLOADER_STACK

	call clearscreen

	push loadingstr
	call print
	inc sp
	inc sp

; read 32 sectors of awesome^W stage2
	mov ax, 0x0220
	mov cx, 2
	xor dx, dx
	mov bx, STAGE2_PHYSADDR
	int 0x13

	jc readerr

;	push 1024
;	push STAGE2_PHYSADDR
;	push numberstr
;	call print
;	hlt

	push memmapstartstr
	call print
	inc sp
	inc sp

; memory mapping
	xor ebx, ebx
	mov di, MEMMAP_PHYSADDR
memmaploop:
	mov edx, 0x534d4150
	mov eax, 0xe820
	mov ecx, 24
	clc
	int 0x15

; stop for a while and print the entry
	add di, 16

	push 4		; dword
	push di		; region type
	push 8		; qword
	sub di, 8
	push di		; region length
	push 8		; qword
	sub di, 8
	push di		; region base
	push memmapentrystr
	call print
	add sp, 14
	
	add di, 24
	test ebx, ebx
	jnz memmaploop

; A20
	in al, 0x92
	or al, 2
	out 0x92, al

	push jumpingtokernelstr
	call print
	inc sp
	inc sp

; disable NMI
	in al, 0x70
	or al, 0x80
	out 0x70, al

; protected mode
	cli
	lgdt [gdt0]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp 0x8:initsegments

bits 32
initsegments:

	mov ax, 16
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov gs, ax
	mov fs, ax

	jmp 0x8:STAGE2_PHYSADDR

bits 16

print:
	pushf
	push eax
	push bx
	push ecx
	push dx
	push di
	push si
	push fs
	push es

	mov ax, SCREEN_SEGMENT
	mov es, ax

	xor eax, eax
	mov si, [ss:esp+24]
	mov di, [print_screenptr]
	mov ecx, 26
	
printloop:
	lodsb
	test al, al
	jz printreturn
	cmp al, '%'
	je printnumber
	cmp al, 10
	je printnextline
	stosb
	inc di
	cmp di, 80*25*2
	jg printcallclrscr
	jmp printloop
printnextline:
	mov ax, di
	mov bl, 80*2
	div bl
	inc al
	cmp al, 26
	je printcallclrscr
	mov bl, 80*2
	mul bl
	mov di, ax
	jmp printloop
printcallclrscr:
	call clearscreen
	xor di, di
	jmp printloop
printnumber:
	mov bx, [ss:esp+ecx]
	mov dx, [ss:esp+ecx+2]
	add ecx, 4
	add bx, dx
	dec bx
printnumberloop:
	test dx, dx
	jz printloop
	mov al, [ds:bx]
	dec bx
	dec dx
printoctet:
	mov fs, ax
	and ax, 0x00F0
	shr ax, 4
	add ax, printdata
	mov al, [ds:eax]
	stosb
	inc di

	mov ax, fs
	and ax, 0x000F
	add ax, printdata
	mov al, [ds:eax]
	stosb
	inc di
	jmp printnumberloop
printdata:
	db	'0123456789abcdef'
printreturn:
	mov [print_screenptr], di
	pop es
	pop fs
	pop si
	pop di
	pop dx
	pop ecx
	pop bx
	pop eax
	popf
	ret
printstaticdata:
print_screenptr		dw	0

clearscreen:
	pushf
	push ax
	push di	
	push es
	mov ax, SCREEN_SEGMENT
	mov es, ax
	xor ax, ax
	xor di, di
clearloop:
	stosb
	inc di
	cmp di, 80*25*2
	jle clearloop
	pop es
	pop di
	pop ax
	popf
	ret

readerr:
	push readerrstr
	call print
	hlt

; GDT data
align 8
gdt0:
	dw	gdtend-gdt0-1
	dd	gdt0	
	times	2	db	0
gdt1:						; code segment
istruc GDT_entry
	.limit_l	dw	0xFFFF
	.base_lw	dw	0x0000
	.base_hwl	db	0x00
	.access_byte	db	10011010b
	.flags_limit	db	11001111b
	.base_hwh	db	0x00
iend
gdt2:						; data segment
istruc GDT_entry
	.limit_l	dw	0xFFFF
	.base_lw	dw	0x0000
	.base_hwl	db	0x00
	.access_byte	db	10010010b
	.flags_limit	db	11001111b
	.base_hwh	db	0x00
iend
gdtend:

loadingstr		db	"Loading...",10,0
memmapstartstr		db	"Memory map (Base/Length/Type):",10,0
memmapentrystr		db	"% % %",10,0
jumpingtokernelstr	db	"Jumping to kernel.",0
readerrstr		db	"Read error.",0
numberstr		db	"%",0

zeropadding:
times 510-($-stage1) db 0

signature:
dw 0xaa55

end:

