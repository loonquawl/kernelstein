#include <allocators.hpp>
#include <assert.hpp>
using namespace Paging;

void BuddyAllocator::Buddy::initializeBuddyMap()
{
	for (Buddy* ptr=m_nextbuddy;
			ptr<sizeof(m_nextbuddy)/sizeof(*m_nextbuddy);
							*ptr++=NULL);
}

BuddyAllocator::Buddy::Buddy(PageSize type, char* region_start)
: m_buddytype(type), m_regionstart(region_start)
{
	freeAllPages();
	initializeBuddyMap();
}

void BuddyAllocator::Buddy::nextFreePage(int8_t& map_qword_num, int8_t& qword_bit_num, )
{
	uint64_t*	map_qwordptr=m_pagemap;
	uint64_t*	map_endptr=m_pagemap+8;
	for (; map_qwordptr<map_endptr && !~*map_qwordptr; ++map_qwordptr);
	// If every page is allocated already
	if (!~*map_qwordptr && map_qwordptr==map_endptr-1)
	{
		map_qword_num=-1;
		qword_bit_num=-1;
	}
	else
	{
		uint64_t	map_qword=*map_qwordptr;
		int8_t		ret_bit_num=0;
		for (; !(map_qword&1); map_qword>>=1,++ret_bit_num);
		if (ret_bit_num==64)
		{
			map_qword_num=-1;
			qword_bit_num=-1;
		}
		else
		{
			map_qword_num=map_qwordptr-m_pagemap;
			qword_bit_num=ret_bit_num;
		}
	}
}

void BuddyAllocator::Buddy::freeAllPages()
{
	kmemset((char*)(m_pagemap),0,sizeof(m_pagemap));
	kmemset((char*)(m_freenextpage),0,sizeof(m_freenextpage));
}

char* BuddyAllocator::Buddy::pageAlloc(PageSize page_size)
{
	uint8_t		map=0;
	uint64_t	map64pages;
	uint8_t		entrybit=0;
	char*		pageaddr;

	// find unused pages
	pageaddr=mapbyte*64*GIGABYTE+entrybit*GIGABYTE;

	if (page_size==buddy_type)
		return pageaddr;

	Buddy** next=&next_buddy[entrybit];
	if (*next==NULL)
	{
		PageSize next_size = (page_size == PAGE1GB) ? PAGE2MB : PAGE4KB;
		*next=new Buddy(next_size,pageaddr);
	}
	return *next->pagealloc(page_size);
}

void BuddyAllocator::Buddy::pagefree(char* ptr)
{
	ASSERT(ptr>region_start && ptr<region_start+(size_t)(buddy_type));
	PageSize page_size = (!(ptr%GIGABYTE)) ? PAGE1GB : (!(ptr%TWOMEGS)) ? PAGE2MB : PAGE4KB;
	if (page_size==buddy_type)
	{
		uint16_t pagenum=(ptr-region_start)/(size_t)(page_size);
		uint8_t mapbyte=pagenum/64;
		uint8_t entrybit=pagenum%64;
		uint64_t mask;
		kmemset((char*)(&mask),0xFF,sizeof(mask));
		mask^=1<<entrybit;		
		next_buddy[mapbyte]&=mask;
	}
	else
	{
		PageSize next_size = (page_size == PAGE1GB) ? PAGE2MB : PAGE4KB;
		unsigned int next_buddy_num=((ptr-region_start)/(size_t)(buddy_type))/(size_t)(next_size);
		Buddy* next=next_buddy[next_buddy_num];
		if (next!=NULL)
			next->pagefree(ptr);
	}
}

char* BuddyAllocator::pagealloc(PageSize size)
{
	Buddy* buddy=gigabuddies;
	for (;buddy<=gigabuddies+gigabuddies_num && buddy->isFull(); ++buddy);
	if (buddy==gigabuddies+gigabuddies_num)
		return NULL;
	
}

