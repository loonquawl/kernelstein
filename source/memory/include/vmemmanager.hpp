#ifndef KERN_VMEMMANAGER
#define KERN_VMEMMANAGER
#include <types.hpp>
#include <stream.hpp>
#include <console.hpp>

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

	class VMemManager
	{
		protected:
			Paging::PML4E		PML4_table[];
			PhysMemAllocator	phys_allocator;
			BIOS::MemoryMapEntry	BIOS_memmap[];

		public:
			VMemManager(Paging::PML4E PML4T[], BIOS::MemoryMapEntry BIOS_memmap);
	
			void print_pagetree(Console& output);
			void print_PML4E(Paging::PML4E*, OStream& output);
			void print_PDPTE(Paging::PDPTE*, OStream& output);
			void print_PDPTE1G(Paging::PDPTE1G*, OStream& output);
			void print_PDE(Paging::PDE*, OStream& output);
			void print_PDE2M(Paging::PDE2M*, OStream& output);
			void print_PTE(Paging::PTE*, OStream& output);

			Paging::PML4E* allocPML4(const Paging::PageFlags* page_flags);
			Paging::PDPTE* allocPDPT(const Paging::PageFlags* page_flags);
			Paging::PDE* allocPD(const Paging::PageFlags* page_flags);
			Paging::PTE* allocPT(const Paging::PageFlags* page_flags);

			void map4K(uint64_t virt_begin, uint64_t phys_begin, const Paging::PageFlags* page_flags);
			void map2M(uint64_t virt_begin, uint64_t phys_begin, const Paging::PageFlags* page_flags);
			void map1G(uint64_t virt_begin, uint64_t phys_begin, const Paging::PageFlags* page_flags);
			void map(uint64_t virt_begin, uint64_t phys_begin, size_t length, const Paging::PageFlags* page_flags);


	};

#endif

