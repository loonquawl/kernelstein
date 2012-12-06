#ifndef KERN_PAGING
#define KERN_PAGING

#define GIGABYTE	0x40000000
#define TWOMEGS		0x200000
#define FOURKB		0x1000

	namespace Paging
	{
		struct PML4E
		{
			uint8_t present			: 1;
			uint8_t	rw			: 1;
			uint8_t	supervisor		: 1;
			uint8_t	write_through		: 1;
			uint8_t cache_disable		: 1;
			uint8_t	accessed		: 1;
			uint8_t	__ignored1		: 1;
			uint8_t	__zero1			: 1;
			uint8_t __ignored2		: 4;
			uint64_t physaddr		: 40;
			uint16_t __ignored3		: 11;
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
			uint8_t is_PDPTE1G		: 1; // has to be 1
			uint8_t global			: 1;
			uint8_t __ignored1		: 3;
			uint8_t PAT			: 1;
			uint32_t __zero1		: 17;
			uint32_t physaddr		: 22;
			uint16_t __ignored2		: 11;
			uint8_t execute_disable		: 1;
		} __attribute__((packed));
	
		// You may not like the way i name these, but it seems i'm
		// through meditating about such mundane problems like
		// type-naming - life's too short.
		union uPDPTE
		{
			PDPTE	pointer_table;
			PDPTE1G	page;
		} __attribute__((packed));

		// Page-Directory Entry (maps page table)
		typedef PML4E PDE;

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
			uint8_t	is_PDE2M		: 1; // has to be 1
			uint8_t global			: 1;
			uint8_t __ignored1		: 3;
			uint8_t PAT			: 1;
			uint8_t __zero1			: 8;
			uint32_t physaddr		: 30;
			uint8_t __zero2			: 1;
			uint16_t __ignored2		: 11;
			uint8_t execute_disable		: 1;
		} __attribute__((packed));

		union uPDE
		{
			PDE	page_table;
			PDE2M	page;
		} __attribute__((packed));

		// Page-Table Entry
		struct PTE
		{
			uint8_t	present			: 1;
			uint8_t	rw			: 1;
			uint8_t	supervisor		: 1;
			uint8_t	write_through		: 1;
			uint8_t	cache_disable		: 1;
			uint8_t	accessed		: 1;
			uint8_t	dirty			: 1;
			uint8_t	PAT			: 1;
			uint8_t	global			: 1;
			uint8_t __ignored1		: 3;
			uint64_t physaddr		: 40;
			uint16_t __ignored2		: 11;
			uint8_t	execute_disable		: 1;
		} __attribute__((packed));

		struct PageFlags
		{
			uint8_t	present			: 1;
			uint8_t	rw			: 1;
			uint8_t	supervisor		: 1;
			uint8_t	write_through		: 1;
			uint8_t	cache_disable		: 1;
			uint8_t	accessed		: 1;
			uint8_t	dirty			: 1;
			uint8_t	PAT			: 1;
			uint8_t	global			: 1;
			uint8_t	execute_disable		: 1;
		};

		enum PageSize
		{
			PAGE4KB = FOURKB,
			PAGE2MB = TWOMEGS,
			PAGE1GB = GIGABYTE
		};
	}

#endif

