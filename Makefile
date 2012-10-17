LD = i386-pc-linux-gnu-ld
CC = gcc
AS = nasm

sourcedir	= source
builddir	= build
kernelopts	= -nostdlib -nodefaultlibs -m32 -fno-builtin
ldopts		= --nostdlib
ldoptsbin	= --oformat binary

all: image.bin

bootloader.bin: $(sourcedir)/bootloader.asm
	$(AS) -f bin -i $(sourcedir)/ -o $(builddir)/bootloader.bin $(sourcedir)/bootloader.asm
	
kernel.o: $(sourcedir)/kernel.c
	$(CC) -O0 -c $(kernelopts) $(sourcedir)/kernel.c -o $(builddir)/kernel.o

kernel.bin: kernel.o
	$(LD) $(ldoptsbin) $(ldopts) -o $(builddir)/kernel.bin -T $(sourcedir)/linkerscripts/kernel

image.bin: bootloader.bin kernel.bin
	cat $(builddir)/bootloader.bin > $(builddir)/image.bin
	cat $(builddir)/kernel.bin >> $(builddir)/image.bin

simulate: image.bin
	qemu -s -fda $(builddir)/image.bin 

clean:
	rm $(builddir)/*
