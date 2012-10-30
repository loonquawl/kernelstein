#include "kernel.hpp"
#include "console.hpp"
#include "string.hpp"
#include "util.h"

extern void* start_ctors;
extern void* end_ctors;

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

	PRINTVAR(console,(long)0xdeadbeef);
	PRINTVAR(console,start_ctors);
	PRINTVAR(console,end_ctors);
	console << 0xdeadbeef << "\n";
	console.printf("%d\n",0xdeadbeef);
	console << 0x8000 << "\n";

	for (long i=0;;++i)
	{
		console << Console::HEX << i << Console::CharColor(i%10) << " ";
	}

	for (;;);
}

