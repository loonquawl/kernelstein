#ifndef KERN_MEMORYMANAGER
#define KERN_MEMORYMANAGER
#include "types.hpp"
#include "stream.hpp"
#include "console.hpp"

#define KCOPY_PAGE_FLAGS(page,page_flags)	\
			{			\
				page->present		= page_flags->present;		\
				page->rw		= page_flags->rw;		\
				page->supervisor	= page_flags->supervisor;	\
				page->write_through	= page_flags->write_through;	\
				page->cache_disable	= page_flags->cache_disable;	\
				page->accessed		= page_flags->accessed;		\
				page->dirty		= page_flags->dirty;		\
				page->PAT		= page_flags->PAT;		\
				page->global		= page_flags->global;		\
				page->execute_disable	= page_flags->execute_disable;	\
			}

#define KCOPY_PAGE_DIRECTORY_FLAGS(page_directory,page_flags)	\
			{					\
				page_directory->present		= page_flags->present;		\
				page_directory->rw		= page_flags->rw;		\
				page_directory->supervisor	= page_flags->supervisor;	\
				page_directory->write_through	= page_flags->write_through;	\
				page_directory->cache_disable	= page_flags->cache_disable;	\
				page_directory->accessed	= page_flags->accessed;		\
			}

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

