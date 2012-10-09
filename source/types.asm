struc IDT_entry
	offset_l:	resw	1
	selector:	resw	1
	zero:		resb	1
	type_attr:	resb	1
	offset_h:	resw	1
endstruc

struc GDT_entry
	.limit_l:		resw	1
	.base_lw:		resw	1	; lower word
	.base_hwl:		resb	1	; higher word lower byte (yes, spaghetti...)
	.access_byte:		resb	1
	.flags_limit:		resb	1	; oh gods, why are the x86 folks doing this to me
	.base_hwh:		resb	1	; MSByte of MSWord of the base
endstruc

struc BIOS_memorymap
	.base_address		resq	1
	.reserved1		resq	1
	.type			resd	1
	.extended_attr		resd	1
endstruc

