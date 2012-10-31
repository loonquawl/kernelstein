#ifndef KERN_PAGEALLOC
#define KERN_PAGEALLOC
#include "types.h"

	class PageAllocator
	{
		public:

			// int15h, ax=0xe820
			struct MemoryMapEntry
			{
				uint64_t base_addr;
				uint64_t length;
				uint8_t	 type;
				uint8_t  reservedb1;
				uint8_t  reservedb2;
				uint8_t  reservedb3;
				uint8_t  reservedb4;
				uint8_t  reservedb5;
				uint8_t  reservedb6;
				uint8_t  reservedb7;
			} __attribute__((packed));

			enum MemoryType
			{
				USABLE = 1,
				RESERVED
			};

			enum PageSize
			{
				P4K,
				P1M
			};

		private:

			PageAllocator() {}
	
		public:
	
			static MemoryMapEntry* const memmap_addr;

			static void* get_page();
	};

#endif

