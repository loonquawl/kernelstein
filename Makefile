LD = i686-pc-linux-gnu-ld
CC = gcc
CP = g++
AS = nasm

sourcedir	= source
builddir	= build
CCopts		= -pipe
CPopts		= -pipe
kernelopts	= -nostdlib -nostartfiles -nodefaultlibs -m32 -ffreestanding -O0 -combine -Wl,-r -fno-rtti -nostdinc++
ldopts		= --nostdlib
ldoptsbin	= --oformat binary

kernelobjects	= $(sourcedir)/*.cpp $(sourcedir)/*.c

all: image.bin

bootloader.bin: $(sourcedir)/bootloader.asm
	$(AS) -f bin -i $(sourcedir)/ -o $(builddir)/bootloader.bin $(sourcedir)/bootloader.asm
	
kernel.o: $(kernelobjects)
	$(CP) $(CPopts) $(kernelopts) $(kernelobjects) -o $(builddir)/kernel.o

kernel.bin: kernel.o
	$(LD) $(ldoptsbin) $(ldopts) -o $(builddir)/kernel.bin -T $(sourcedir)/linkerscripts/kernel

image.bin: bootloader.bin kernel.bin
	cat $(builddir)/bootloader.bin > $(builddir)/image.bin
	cat $(builddir)/kernel.bin >> $(builddir)/image.bin

simulate: image.bin
	qemu -s -fda $(builddir)/image.bin 

clean:
	rm $(builddir)/*

