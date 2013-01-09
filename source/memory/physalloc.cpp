#include <allocators.hpp>
#include <assert.hpp>
using namespace Paging;

void BuddyAllocator::Buddy::initializeBuddyMap()
{
	// Set all nextbuddyptrs to NULL
	for (Buddy* ptr=m_nextbuddy;
			ptr<sizeof(m_nextbuddy)/sizeof(*m_nextbuddy);
							*ptr++=NULL);
}

BuddyAllocator::Buddy::Buddy(PageSize type, char* region_start)
: m_buddysize(type), m_regionstart(region_start)
{
	freeAllPages();
	initializeBuddyMap();
	kmemset((char*)(m_disabledpages),0,sizeof(m_disabledpages));
}

void BuddyAllocator::Buddy::nextFreePage(int8_t* p_qword_offset, int8_t* p_bit_offset, char** p_pageaddr)
{
	// 8*64 of pages, one bit per page
	uint64_t*	map_qwordptr=m_pagemap;
	// Points to one qword after end of pagemap.
	uint64_t*	map_endptr=m_pagemap+8;
	// Go to 64-page chunk in which at least one page is free.
	for (; map_qwordptr<m_pagemap+ELEMENTS(m_pagemap); && !~*map_qwordptr; ++map_qwordptr);
	// If every page is already allocated.
	if (map_qwordptr==map_endptr)
	{
		// We're dooomed!
		if (p_qword_offset!=NULL)
			*p_qword_offset=-1;
		if (p_bit_offset!=NULL)
			*p_bit_offset=-1;
		if (p_pageaddr!=NULL)
			*p_pageaddr=NULL;
	}
	else
	{
		// ...or not so
		// Take the qword in question and find the free page.
		uint64_t	map_qword=*map_qwordptr;
		// And remember its position, so we can compute pageaddr later.
		int8_t		bit_offset=0;
		for (; !(map_qword&1); map_qword>>=1,++bit_offset);

		if (p_qword_offset!=NULL)
			*p_qword_offset=map_qwordptr-m_pagemap;
		if (p_bit_offset!=NULL)
			*p_bit_offset=bit_offset;
		if (p_pageaddr!=NULL)
			*p_pageaddr=(map_qwordptr-m_pagemap)*64*GIGABYTE+bit_offset*GIGABYTE+m_regionstart;
	}
}

void BuddyAllocator::Buddy::freeAll()
{
	kmemset((char*)(m_pagemap),0,sizeof(m_pagemap));
	for (Buddy** bptr=m_nextbuddy; bptr<m_nextbuddy+ELEMENTS(m_nextbuddy); ++bptr)
		if (*bptr!=NULL)
			delete *bptr;	// TODO check allocator types
}

char* BuddyAllocator::Buddy::alloc(PageSize page_size)
{
	kassert((size_t)(page_size)>=(size_t)(m_buddysize));
	// Try to find next free page;
	int8_t	qword_offset;
	int8_t	bit_offset;
	char*	pageaddr;
	nextFreePage(&qword_offset,&bit_offset,&pageaddr);
	if (qword_offset==-1)
	{
		// Sorry, we're closed.
		return NULL;
	}
	if (page_size!=m_buddysize)
	{
		// Pass it to the next buddy.
		Buddy** pp_nextbuddy=&m_nextbuddy[qword_offset*64+bit_offset];
		if (*pp_nextbuddy==NULL)
		{
			// Alloc next buddy, if there isn't one.
			PageSize nextbuddy_size = m_buddysize == PAGE1GB ? PAGE2M : PAGE4K;
			char* nextbuddy_regionstart=m_regionstart+64*qword_offset*(size_t)(m_buddysize)
							+bit_offset*(size_t)(m_buddysize);
			*pp_nextbuddy = new Buddy(nextbuddy_size,nextbuddy_regionstart);
			return *pp_nextbuddy->alloc(page_size);
		}
	}
	return pageaddr;
}

void BuddyAllocator::Buddy::free(char* ptr)
{
	
}

bool BuddyAllocator::Buddy::isFull()
{
	for (uint64_t* pptr=m_pagemap; pptr<m_pagemap+ELEMENTS(m_pagemap); ++pptr)
		if (~*pptr)
			return false;
	return true;
}

char* BuddyAllocator::alloc(PageSize page_size)
{
	Buddy* buddy=m_gigabuddies;
	for (;buddy<m_gigabuddies+gigabuddies_num && buddy->isFull(); ++buddy);
	if (buddy==m_gigabuddies+gigabuddies_num)
		return NULL;
	return buddy->alloc(page_size);
}

void BuddyAllocator::free(char* ptr)
{
	
}

BuddyAlloc::BuddyAlloc(char* region_start, size_t region_size)
 m_regionstart(region_start), m_regionsize(region_size)
{
	m_gigabuddiesnum=region_size/GIGABYTE + (region_size%GIGABYTE!=0);
	m_gigabuddies = new Buddy*[m_gigabuddiesnum];
	for (Buddy* ptr=m_gigabuddies; ptr<m_gigabuddies+m_gigabuddiesnum; *ptr++=NULL);
	// TODO set_upper_bound
}

