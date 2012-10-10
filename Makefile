LD = i386-pc-linux-gnu-ld
CC = gcc

sourcedir	= source
builddir	= build
kernelopts	= -nostdlib -nodefaultlibs -m32 -fno-builtin
ldoptsbin	= --oformat binary --nostdlib

all: image.bin

bootloader.o: $(sourcedir)/bootloader.asm
	nasm -w+gnu-elf-extensions -f elf32 -i $(sourcedir)/ -o $(builddir)/bootloader.o $(sourcedir)/bootloader.asm
	
bootloader.bin: bootloader.o
	$(LD) $(ldoptsbin) -T $(sourcedir)/linkerscripts/bootloader -o $(builddir)/bootloader.bin $(builddir)/bootloader.o
    
kernel.o: $(sourcedir)/kernel.c
	$(CC) -c $(kernelopts) $(sourcedir)/kernel.c -o $(builddir)/kernel.o

clean:
	rm $(builddir)/*

image.bin: bootloader.o kernel.o
	$(LD) $(ldoptsbin) -T $(sourcedir)/linkerscripts/image -o $(builddir)/image.bin

simulate: image.bin
	qemu -s $(builddir)/image.bin
