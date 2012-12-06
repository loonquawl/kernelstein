#ifndef KERN_BIOS
#define KERN_BIOS

	namespace BIOS
	{
		// old-fashioned int15h, ax=0xE820 memory map
		struct MemoryMapEntry
		{
			uint64_t base_addr;
			uint64_t length;
			uint8_t	 type;
			uint8_t	 reserved[7];
		} __attribute__((packed));

		enum MemoryType
		{
			USABLE = 1,
			RESERVED
		};
	};

#endif

