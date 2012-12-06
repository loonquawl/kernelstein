#include "allocators.hpp"
using namespace Paging;

char* Buddy::alloc(PageSize page_size)
{
	uint8_t mapiter=0;
	uint64_t mapentry;
	uint8_t entrynum=0;

	for (; !page_map[mapiter]; ++mapiter);
	mapentry=page_map[mapiter];
	for (; !(mapentry&1); mapentry>>=1,++entrynum);

	if (page_size == buddy_type)
		return region_start+entrynum*(size_t)(page_size);
	
}

void Buddy::free(char* ptr)
{

}

