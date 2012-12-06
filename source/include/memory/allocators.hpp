#ifndef KERN_ALLOCATORS
#define KERN_ALLOCATORS
#include <types.hpp>
#include <util.hpp>
#include <nullptr.hpp>

	class PhysMemAllocator
	{
		public:
			virtual char* alloc(Paging::PageSize size) = 0;
			virtual void free(char* ptr) = 0;
	};

	class BuddyAlloc : public PhysMemAllocator
	{
		protected:

			class Buddy
			{
				protected:				
					uint64_t	page_map[8];
					Buddy*		next_buddy[8*64];
					PageSize	buddy_type;
					size_t		region_start;

				public:
					Buddy(PageSize type=PAGE1GB, size_t region_start)
					: buddy_type(type), region_start(region_start)
					{
						kmemset((char*)(page_map),0xFF,sizeof(page_map));
						for (Buddy* ptr=next; ptr<sizeof(next)/sizeof(*next); *ptr++=NULL);
					}

					char* alloc(Paging::PageSize size);
					void free(char* ptr);
			};

			char*	memory;
			size_t	size;

			uint64_t gigabyte_pages[];
			uint64_t 

		public:
			BuddyAlloc();

			char*	alloc(size_t size);
			void	free(char* ptr);
	};

#endif

