#include "kernel.hpp"
#include "console.hpp"

const char* cs_kernelgreeting="W e l c o m e   t o   K e r n e l s t e i n ! ";

void kernelentry()
{
	EarlyKernelConsole console;

	console.print("Hello!\n");
	console.set_color(Console::RED,Console::BLACK);
	console.print("Hello Red!");
	console.set_color(Console::MAGENTA,Console::YELLOW);
	console.print("Hello some-other-color without nextline!\n");
	console.set_color(Console::LIGHTBLUE,Console::BLACK);
	console.print("And now some fascinating nextlines!\n\n\n");
	console.printf("And now kprintf crunching numbers and strings!\n");
	//console.printf("2x String: %s | %s\n","Hello","World");
	console.printf("2x Decimal: %d | %d\n",13,37);
	console.print("That's all, folks!\n");

	for (;;);
}

