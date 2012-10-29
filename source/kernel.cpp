#include "kernel.hpp"
#include "console.hpp"
#include "string.hpp"

void* start_ctors=(void*)0xdeadbeef;

void kernelentry()
{
	EarlyKernelConsole console;
	console << "Hello!\n";
	console << Console::RED << "Constructors are at: " << 0xdeadbeef << "\n";
	console.printf("\n\n%d\n\n",(unsigned long)start_ctors);

	for (;;);
}

