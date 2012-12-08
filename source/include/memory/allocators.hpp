#ifndef KERN_ALLOCATORS
#define KERN_ALLOCATORS
#include <types.hpp>
#include <util.hpp>
#include <nullptr.hpp>

	class PhysMemAllocator
	{
		public:
			/*
			 * @brief	Returns page of a given size.
			 * @return 	Page address or NULL.
			 */
			virtual char*	pageAlloc(Paging::PageSize size) = 0;
			/*
			 * @brief	Frees page.
			 */
			virtual void	pageFree(char* address) = 0;

			/*
			 * @brief	Returns address of a contiguous memory region
			 * 		(several pages one after another).
			 * @return	Address or NULL.
			 */
			virtual char*	alloc(size_t size);
			/*
			 * @brief	Frees all allocated contiguous pages.
			 */
			virtual void	free(char* address);
	};

	class BuddyAllocator : public PhysMemAllocator
	{
		protected:
			class Buddy
			{
				protected:
					/*
					 * @brief	Bit lit -> page used
					 */
					uint64_t	m_pagemap[8];
					uint64_t	m_disabledpages[8];
					/*
					 * @brief	When you alloc a contiguous memory
					 * 		region consisting of several pages.
					 */
					uint64_t	m_freenextpage[8];
					Buddy*		m_nextbuddy[8*64];
					PageSize	m_buddytype;
					char*		m_regionstart;

					void	initializeBuddyMap();					
					void	nextFreePage(uint8_t* map_qword_num=NULL, uint8_t& qword_bit_num=NULL);
	
				public:
					Buddy(PageSize type=PAGE1GB, char* region_start);

					void	freeAllPages();
					char*	pageAlloc(Paging::PageSize size);
					void	pageFree(char* ptr);

					/*
					 * @brief Pages after address will be disabled.
					 * TODO
					 */
					void setUpperBound(char* address);

					bool isFull()
					{
						return !~m_pagemap;
					}
			};
	

			char*	m_regionstart;
			size_t	m_regionsize;

			Buddy* m_gigabuddies[];
			size_t m_gigabuddiesnum;

		public:
			BuddyAlloc(char* region_start, size_t region_size)
			: m_regionstart(region_start), m_regionsize(region_size)
			{
				m_gigabuddiesnum=region_size/GIGABYTE + (region_size%GIGABYTE!=0);
				m_gigabuddies = new Buddy*[m_gigabuddiesnum];
				for (Buddy* ptr=m_gigabuddies; ptr<m_gigabuddies+m_gigabuddiesnum; *ptr++=NULL);
				// TODO set_upper_bound
			}

			char*	pageAlloc(Paging::PageSize size);
			void	pagefree(char* ptr);
	};

#endif

