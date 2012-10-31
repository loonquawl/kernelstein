#include "pageallocator.hpp"
#include "nullptr.h"

PageAllocator::MemoryMapEntry* const PageAllocator::memmap_addr = (MemoryMapEntry*)(0x500);

void* PageAllocator::get_page()
{

}

