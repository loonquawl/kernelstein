#include "kernel.hpp"
#include "string.hpp"
#include "util.hpp"
#include "cxx-api.h"
#include "globals.hpp"
#include "panic.hpp"
#include "memorymanager.hpp"

void kernelentry()
{
//	kclihlt();
	kconstruct_globals();
	MemoryManager::print_pagetree(g_console);

	kclihlt();
	unsigned long* ptr=(unsigned long*)(0xb0bca7c0de);
	for (; !*ptr; ++ptr);
	g_console << HexStr((unsigned long)(ptr)) << "\t" << HexStr(*ptr) << "\t";
}

