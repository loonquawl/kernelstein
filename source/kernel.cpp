#include "kernel.hpp"
#include "string.hpp"
#include "util.h"
#include "cxx-api.h"
#include "globals.hpp"
#include "panic.hpp"
#include "pageallocator.hpp"

void kernelentry()
{
//	kclihlt();
	kconstruct_globals();

	g_console << "Memory map:" << "\n";

	g_console << Console::HEX;
	for (PageAllocator::MemoryMapEntry* eptr=PageAllocator::memmap_addr;
		eptr->type; ++eptr)
	{
		g_console	<< eptr->base_addr	<< "\t"
				<< eptr->length		<< "\t"
				<< eptr->type		<< "\n";
	}
}

