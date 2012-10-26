#include "kernel.hpp"
#include "console.hpp"
#include "string.hpp"

EarlyKernelConsole console;

void kernelentry()
{
	console << "Hello!\n";

	for (;;);
}

