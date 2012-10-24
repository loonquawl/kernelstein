LD = i686-unknown-elf-ld
CC = i686-unknown-elf-gcc
CP = g++
AS = nasm

sourcedir	= source
builddir	= build
CCopts		= -pipe
CPopts		= -pipe
kernelopts	= -nostdlib -nostartfiles -nodefaultlibs -m32 -ffreestanding -O0 -combine -Wl,-r -fno-rtti -nostdinc++ -ffunction-sections -fno-threadsafe-statics -Wabi
ldopts		= --nostdlib
ldoptsbin	= --oformat binary

kernelobjects	= $(sourcedir)/*.cpp $(sourcedir)/*.c

all: image.bin kernel_relocated.o

bootloader.bin: $(sourcedir)/bootloader.asm
	$(AS) -f bin -i $(sourcedir)/ -o $(builddir)/bootloader.bin $(sourcedir)/bootloader.asm
	
kernel.o: $(kernelobjects)
	$(CP) $(CPopts) $(kernelopts) $(kernelobjects) -o $(builddir)/kernel.o 

kernel_relocated.o: kernel.o
	$(LD) $(ldopts) -o $(builddir)/kernel_relocated.o -T $(sourcedir)/linkerscripts/kernel

kernel.bin: kernel.o
	$(LD) $(ldoptsbin) $(ldopts) -o $(builddir)/kernel.bin -T $(sourcedir)/linkerscripts/kernel

image.bin: bootloader.bin kernel.bin
	cat $(builddir)/bootloader.bin > $(builddir)/image.bin
	cat $(builddir)/kernel.bin >> $(builddir)/image.bin
	# qemu doesn't seem to read last sector if it isn't full 512b
	dd if=/dev/zero bs=512 count=1 >> $(builddir)/image.bin

simulate: image.bin
	qemu -s -fda $(builddir)/image.bin 

clean:
	rm $(builddir)/*

