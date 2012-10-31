#ifndef KERN_PAGEALLOC
#define KERN_PAGEALLOC
#include "types.h"

	class PageAllocator
	{
		public:

			// int15h, ax=0xe820
			struct MemoryMapEntry
			{
				uint32_t base_address;
				uint32_t reserved;
				uint16_t type;
				uint16_t extended_attr;
			};

		private:

			PageAllocator() {}
	
			static const MemoryMapEntry* memmap_addr;

		public:
	
			enum PageSize
			{
				P4K,
				P1M
			};


			static void* get_page();
	};

#endif

