#include "kernel.hpp"
#include "console.hpp"
#include "string.hpp"
#include "util.h"

extern void* start_ctors;
extern void* end_ctors;
extern void* entry;

class Test
{
	int abc;
	public:
		Test()
		{
			abc=abc*2;
		}
};

Test blah;

void kernelentry()
{
	EarlyKernelConsole console;

	char ps2commport=0x64;
	char ps2dataport=0x60;
	char outbyte=0xd0; // test controller
	char inbyte=0; // should be PS2 controller output port

	koutb(ps2commport,outbyte);
	inbyte=kinb(ps2dataport+1);

	console << Console::HEX << inbyte << "\n";

	for (;;);
}

