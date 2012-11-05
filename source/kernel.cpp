#include "kernel.hpp"
#include "string.hpp"
#include "util.h"
#include "cxx-api.h"
#include "globals.hpp"
#include "panic.hpp"
#include "memorymanager.hpp"

void kernelentry()
{
//	kclihlt();
	kconstruct_globals();
	MemoryManager::PML4E abc;
}

