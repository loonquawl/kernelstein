%include "types.asm"
%define SCREEN_SEGMENT		0xb800
%define STAGE1_PHYSADDR		0x7c00
%define STAGE2_PHYSADDR		0x8000
%define STAGE2_VIRTADDR		0xb0bc47c0de
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

	; load rest of 1024b bootloader
	mov ax, 2
	mov bx, STAGE1_PHYSADDR+512
	call readsector
	inc sp
	inc sp
	
	; load the kernel
	mov ax, 3
	mov bx, STAGE2_PHYSADDR
	mov cx, 512 ; 256kb
load_kern:
	call readsector
	inc sp
	inc sp
	inc ax
	loop load_kern

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

	push jumpingtokernelstr
	call print
	inc sp
	inc sp

; ax - sector number, es:bx - output
readsector:
	push ax
	push cx
	push dx

	mov ax, [sp+8]
	dec ax
	mov cl, 36
	xor dx, dx
	div cl
	mov ch, al
	cmp ah, 18
	jb secnum_ok
	sub ah, 18
	inc dh
secnum_ok:
	mov cl, ah
	mov ax, 0x0201
	inc cl

	push dx
	stc
	int 0x13
	jc readerr
	pop dx

	pop dc
	pop cx
	pop ax

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
	mov ax, 0x0720
	xor di, di
clearloop:
	stosw
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

gdt:
gdt0:
	dw	gdt_end-gdt0-1
	dd	gdt0	
	times	2	db	0
gdt1:						; code segment
Code32	equ	$-gdt
istruc GDT_entry
	.limit_l	dw	0xFFFF
	.base_lw	dw	0x0000
	.base_hwl	db	0x00
	.access_byte	db	10011010b
	.flags_limit	db	11001111b
	.base_hwh	db	0x00
iend
gdt2:						; data segment
Data32	equ	$-gdt
istruc GDT_entry
	.limit_l	dw	0xFFFF
	.base_lw	dw	0x0000
	.base_hwl	db	0x00
	.access_byte	db	10010010b
	.flags_limit	db	11001111b
	.base_hwh	db	0x00
iend
gdt3:						; 64 code segment
Code64	equ	$-gdt
istruc GDT_entry
	.limit_l	dw	0
	.base_lw	dw	0
	.base_hwl	db	0
	.access_byte	db	10011010b
	.flags_limit	db	00100000b
	.base_hwh	db	0
iend
gdt4:						; 64 data segment
Data64	equ	$-gdt
istruc GDT_entry
	.limit_l	dw	0
	.base_lw	dw	0
	.base_hwl	db	0
	.access_byte	db	10010010b
	.flags_limit	db	00100000b
	.base_hwh	db	0
gdt_end:

protmode:
	; A20
	in al, 0x92
	or al, 2
	out 0x92, al

	; disable NMI
	in al, 0x70
	or al, 0x80
	out 0x70, al

	; load GDT	
	cli
	lgdt [gdt]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp Code32:initsegments

bits 32
initsegments:

	mov ax, 16
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov gs, ax
	mov fs, ax

setuppaging:
	; clear tables
	mov edi, 0x1000
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3
	; init tables
	mov dword [edi], 0x2003
	add edi, 0x1000
	mov dword [edi], 0x3003
	add edi, 0x1000
	mov dword [edi], 0x4003
	add edi, 0x1000
	; identity map first MB
	mov ebx, 3
	mov ecx, 512
setentry:
	mov dword [edi], ebx
	add ebx, 0x1000
	add edi, 8
	loop setentry

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax
	; enable LM bit
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr
	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

;	lgdt [gdt]
	jmp Code64:Realm64

bits 64
Realm64:
	mov rax, 0xdeadbeefc0debabe
	mov [0xb8000], rax
	hlt

zeropadding2:
times 1024-($-stage1) db 0

end:

