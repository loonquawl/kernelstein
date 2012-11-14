LD = ld
CC = gcc
CP = g++
AS = nasm
EMU = qemu-system-x86_64

SOURCEDIR	= source
BUILDDIR	= build
CFLAGS		= -pipe -Wno-packed-bitfield-compat
CPFLAGS		= -pipe -Wno-packed-bitfield-compat
KERNELFLAGS	= -nostdlib -nostartfiles -nodefaultlibs -m64 -ffreestanding -O2 -combine -Wl,-r -fno-rtti -fno-exceptions -nostdinc++ -ffunction-sections -fno-threadsafe-statics -Wabi -ggdb -mno-sse -mno-sse2 -mno-mmx -mno-3dnow -mcmodel=large
LDFLAGS		= --nostdlib
LDFLAGSBIN	= --oformat binary
EMUFLAGS	= --no-kvm 

OBJECTS		= $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*.c)

$(shell mkdir -p $(BUILDDIR))

all: image.bin kernel_relocated.o

bootloader.bin: $(SOURCEDIR)/bootloader.asm
	$(AS) -f bin -I $(SOURCEDIR)/include/ -o $(BUILDDIR)/bootloader.bin $(SOURCEDIR)/bootloader.asm

kernel.o: $(KERNELOBJECTS)
	$(CP) $(CPFLAGS) $(KERNELFLAGS) $(OBJECTS) -I$(SOURCEDIR)/include -o $(BUILDDIR)/kernel.o 

kernel_relocated.o: kernel.o
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel_relocated.o -T $(SOURCEDIR)/linkerscripts/kernel $(BUILDDIR)/kernel.o

kernel.bin: kernel.o
	$(LD) $(LDFLAGSBIN) $(LDFLAGS) -o $(BUILDDIR)/kernel.bin -T $(SOURCEDIR)/linkerscripts/kernel $(BUILDDIR)/kernel.o

image.bin: bootloader.bin kernel.bin
	cat $(BUILDDIR)/bootloader.bin > $(BUILDDIR)/image.bin
	cat $(BUILDDIR)/kernel.bin >> $(BUILDDIR)/image.bin
	# qemu doesn't seem to read last sector if it isn't full 512b
	dd if=/dev/zero bs=512 count=1 >> $(BUILDDIR)/image.bin

simulate: image.bin
	$(EMU) $(EMUFLAGS) -s -fda $(BUILDDIR)/image.bin 

debug: image.bin
	$(EMU) $(EMUFLAGS) -S -s -fda $(BUILDDIR)/image.bin

gdb:
	gdb -x misc/gdbstartup

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean simulate debug gdb

