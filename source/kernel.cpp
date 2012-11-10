#include "kernel.hpp"
#include "string.hpp"
#include "util.hpp"
#include "cxx-api.h"
#include "globals.hpp"
#include "panic.hpp"
#include "memorymanager.hpp"
#include "assert.hpp"

void kernelentry()
{
//	kclihlt();
	kconstruct_globals();
	kpanic("panic test");
	ASSERT(*(uint64_t*)(0xabcd)==0xbeef);
	MemoryManager::print_pagetree(g_console);

	unsigned long* ptr=(unsigned long*)(0xb0bca70000);
	for (; !*ptr; ++ptr);
	g_console << HexStr((unsigned long)(ptr)) << "\t" << HexStr(*ptr) << "\t";
}

