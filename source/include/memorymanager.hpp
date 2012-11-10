#ifndef KERN_MEMORYMANAGER
#define KERN_MEMORYMANAGER
#include "types.hpp"
#include "stream.hpp"
#include "console.hpp"

	class MemoryManager
	{
		public:

			enum PageSize
			{
				P4K,
				P2M,
				P1G
			};			

		protected:

			MemoryManager() {}

			static Paging::PML4E* PML4T;
	
		public:
	
			static BIOS::MemoryMapEntry* const BIOS_memmap_addr;

			static void* get_page(PageSize size);
			static void map_page();

			static void print_pagetree(Console& output);
			static void print_PML4E(Paging::PML4E*, OStream& output);
			static void print_PDPTE(Paging::PDPTE*, OStream& output);
			static void print_PDPTE1G(Paging::PDPTE1G*, OStream& output);
			static void print_PDE(Paging::PDE*, OStream& output);
			static void print_PDE2M(Paging::PDE2M*, OStream& output);
			static void print_PTE(Paging::PTE*, OStream& output);

			static void map4K(uint64_t virt_begin, uint64_t phys_begin, const Paging::PTE* page_flags);
			static void map2M(uint64_t virt_begin, uint64_t phys_begin, const Paging::PDE2M* page_flags);
			static void map1G(uint64_t virt_begin, uint64_t phys_begin, const Paging::PDPTE1G* page_flags);
			static void map(uint64_t virt_begin, uint64_t phys_begin, size_t length);
	};

#endif

