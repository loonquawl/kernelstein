#include "kernel.hpp"
#include "console.hpp"
#include "string.hpp"

void kernelentry()
{
	EarlyKernelConsole console;
	console << "Hello!\n";

	for (;;);
}

