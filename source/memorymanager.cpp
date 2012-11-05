#include "memorymanager.hpp"
#include "nullptr.h"

MemoryManager::MemoryMapEntry* const MemoryManager::memmap_addr = (MemoryMapEntry*)(0x500);
MemoryManager::PML4E *PML4T[512] = (*PML4E[512])(0x1000);

void* MemoryManager::get_page()
{

}

void MemoryManager::print_pagetree()
{
	
}

