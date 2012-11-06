%include "types.asm"
%define SCREEN_SEGMENT		0xb800
%define STAGE1_PHYSADDR		0x7c00
%define STAGE2_PHYSADDR		0x8000
%define STAGE2_VIRTADDR		0xb0bc47c0de0000
%define BOOTLOADER_STACK	0x7bfe
%define MEMMAP_PHYSADDR		0x0500
%define PML4T_PHYSADDR		0x1000

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
	push 512
	push STAGE1_PHYSADDR/16
	push 2
	call readsector
	add sp, 6
	
	; load the kernel
	mov ax, 3
	mov bx, (STAGE1_PHYSADDR+1024)/16
	mov cx, 1024				; 512kb
load_kern:
	push 0					; offset
	push bx					; segment
	push ax					; sector number
	call readsector
	add sp, 6

	inc ax
	add bx, 512/16

	push dotstr
	call print
	inc sp
	inc sp

	loop load_kern

	push memmapstartstr
	call print
	inc sp
	inc sp

memmap:
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

	push 8		; qword
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

	; set last entry to all zeroes
	xor eax, eax
	mov ecx, 6
zerolastentry:
	stosd
	loop zerolastentry

	jmp protmode

; word [esp+18] - bx \
; word [esp+16] - es  -	output es:bx
; word [esp+14] - sector number
readsector:
	pushf
	push ax
	push bx
	push cx
	push dx
	push es

	mov ax, [ss:esp+16]
	mov es, ax

	mov bx, [ss:esp+18]

	mov ax, [ss:esp+14]
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

	stc
	int 0x13
	jc readerr

	pop es
	pop dx
	pop cx
	pop bx
	pop ax
	popf
	ret

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

loadingstr		db	"Loading",0
memmapstartstr		db	10,"Memory map (Base/Length/Type):",10,0
memmapentrystr		db	"% % %"
newlinestr		db	10,0
readerrstr		db	"Read error",0
;numberstr		db	"%",0
dotstr			db	".",0

zeropadding:
times 510-($-stage1) db 0

signature:
dw 0xaa55

leaverealstr		db	"Jumping to kernel",0

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

nolongmodestr	db	"Long mode not available.    ",0

bits 32
paintitred32:
	push edi
	mov edi, 0xb8001
	mov al, 14+64
paintitred32_loop:
	stosb
	inc edi
	cmp edi, 0xb8001+80*25*2
	jne paintitred32_loop
	pop edi
	ret

print32:
	push esi
	push edi
	mov esi, [esp+12]
	mov edi, 0xb8000
print32loop:
	lodsb
	test al, al
	jz print32end
	stosb
	inc edi
	jmp print32loop
print32end:
	pop edi
	pop esi
	ret

bits 16
protmode:
	; A20
	in al, 0x92
	or al, 2
	out 0x92, al

	; disable NMI
	in al, 0x70
	or al, 0x80
	out 0x70, al

	push leaverealstr
	call print
	inc sp
	inc sp

	; load GDT	
	cli
	lgdt [gdt]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp Code32:initsegments

bits 32
initsegments:
	mov ax, Data32
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov gs, ax
	mov fs, ax

check64bit:
	mov eax, 0x80000001
	cpuid
	and edx, 1 << 29
	jnz setuppaging

	call paintitred32

	push nolongmodestr
	call print32
	hlt

setuppaging:
	; clear tables
	mov edi, PML4T_PHYSADDR
	mov cr3, edi
	xor eax, eax
	mov ecx, 4096
	rep stosd
	mov edi, cr3
	; init tables
	; PML4T
	mov dword [edi], PML4T_PHYSADDR+0x1003
	; PDPT
	add edi, 0x1000
	mov dword [edi], PML4T_PHYSADDR+0x2003
	; PDE
	add edi, 0x1000	
	mov dword [edi], PML4T_PHYSADDR+0x3003

	; PT
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
	jmp Code64:initsegments64

bits 64
initsegments64:
	mov ax, Data64
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov gs, ax
	mov fs, ax

; later...
;setupkernelpages:
	; calculate PML4E address
;	mov rbx, cr3
;	mov rax, STAGE2_VIRTADDR
;	shr rax, 39
;	shl rax, 3
;	add rax, rbx
;	mov [rax], 0xb


jumptokernel:
	call 0x8000
	hlt

zeropadding2:
times 1024-($-stage1) db 0

end:

