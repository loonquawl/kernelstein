#ifndef KERN_MEMORYMANAGER
#define KERN_MEMORYMANAGER
#include "types.h"

	class MemoryManager
	{
		public:

			// int15h, ax=0xe820
			struct MemoryMapEntry
			{
				uint64_t base_addr;
				uint64_t length;
				uint8_t	 type;
				uint8_t  reserved[7];
			} __attribute__((packed));

			enum MemoryType
			{
				USABLE = 1,
				RESERVED
			};

			// All of them IA-32e

			struct PML4E
			{
				uint8_t present			: 1;
				uint8_t	rw			: 1;
				uint8_t	supervisor		: 1;
				uint8_t	write_through		: 1;
				uint8_t cache_disable		: 1;
				uint8_t	accessed		: 1;
				uint8_t	ignored1		: 1;
				uint8_t	zero1			: 1;
				uint8_t ignored2		: 4;
				uint64_t physaddr		: 5*8;
				uint16_t ignored3		: 11;
				uint8_t execute_disable		: 1;
			} __attribute__((packed));

			// Page-Directory-Pointer-Table Entry (maps pointer table)
			typedef PML4E PDPTE;

			// Page-Directory-Pointer-Table Entry (maps 1GB page)
			struct PDPTE1G
			{
				uint8_t present			: 1;
				uint8_t rw			: 1;
				uint8_t supervisor		: 1;
				uint8_t write_through		: 1;
				uint8_t cache_disable		: 1;
				uint8_t accessed		: 1;
				uint8_t dirty			: 1;
				uint8_t page_size		: 1; // has to be 1
				uint8_t global			: 1;
				uint8_t ignored1		: 3;
				uint8_t PAT			: 1;
				uint32_t zero1			: 17;
				uint32_t physaddr		: 22;
				uint16_t ignored2		: 11;
				uint8_t execute_disable		: 1;
			};

			// You may not like the way i name these, but it seems i'm
			// through meditating about such mundane problems like
			// type-naming - life's too short.
			union uPDPTE
			{
				PDPTE	pointer_table;
				PDPTE1G	page;
			};

			// Page-Directory Entry (maps page table)
			typedef PDE PDPTE;

			// Page-Directory Entry (maps 2M page)
			struct PDE2M
			{
				uint8_t present			: 1;
				uint8_t rw			: 1;
				uint8_t	supervisor		: 1;
				uint8_t	write_through		: 1;
				uint8_t	cache_disable		: 1;
				uint8_t	accessed		: 1;
				uint8_t	dirty			: 1;
				uint8_t	page_size		: 1; // has to be 1
				uint8_t global			: 1;
				uint8_t ignored1		: 3;
				uint8_t PAT			: 1;
				uint8_t zero1			: 8;
				uint32_t physaddr		: 30;
				uint8_t zero2			: 1;
				uint8_t ignored2		: 11;
				uint8_t execute_disable		: 1;
			};

			union uPDE
			{
				PDE	page_table;
				PDE2M	page;
			};

			// Page-Table Entry
			struct PTE
			{
				uint8_t readbit			: 1;
				uint8_t writebit		: 1;
				uint8_t	executebit		: 1;
				uint8_t	EPT_memory_type		: 3;
				uint8_t	ignore_PAT		: 1;
				uint8_t	ignore			: 1;
				uint8_t	accessed		: 1;
				uint8_t	dirty			: 1;	
				uint8_t	reserved_ignored_1	: 2;
				uint64_t physaddr		: 4+6*8;
			} __attribute__((packed));

			enum PageSize
			{
				P4K,
				P2M,
				P1G
			};			

		private:

			MemoryManager() {}
	
		public:
	
			static MemoryMapEntry* const memmap_addr;
			static PML4E *PML4T[512];

			static void* get_page();

			static void map_page();

			static void print_pagetree();
	};

#endif

