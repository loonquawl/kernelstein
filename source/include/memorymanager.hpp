#ifndef KERN_MEMORYMANAGER
#define KERN_MEMORYMANAGER
#include "types.h"
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
	};

#endif

