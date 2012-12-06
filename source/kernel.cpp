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
	MemoryManager::print_pagetree(g_console);
}

