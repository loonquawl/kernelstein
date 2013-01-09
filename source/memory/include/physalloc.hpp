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
			virtual char*	alloc(Paging::PageSize size) = 0;

			/*
			 * @brief	Frees page.
			 */
			virtual void	free(char* address) = 0;
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

					/*
					 * @brief	Where be dragons
					 */
					uint64_t	m_disabledpages[8];

					/*
					 * @brief	Pointers to buddies responsibru for assigning smaller pages
					 */
					Buddy*		m_nextbuddy[8*64];

					/*
					 * @brief	A buddy can assign 1GB, 2MB or 4KB pages
					 */
					PageSize	m_buddysize;

					/*
					 * @brief	Page addresses start there.
					 */
					char*		m_regionstart;

					/*
					 * @brief	Sets every nextbuddyptr to NULL.
					 */
					void	initializeBuddyMap();

					/*
					 * @brief	Returns internal map position of next free page of size
					 * 		m_buddysize.
					 */
					void	nextFreePage(uint8_t* p_qword_offset=NULL, uint8_t* p_bit_offset=NULL, char** p_pageaddr);
	
				public:
					/*
					 * @param type		Maximum size of page allocated by the buddy.
					 * @param region_start	Which part of physical memory to manage?
					 */
					Buddy(PageSize type, char* region_start);

					/*
					 * @brief	Frees all pages, destroys underlying buddies.
					 */
					void	freeAll();

					/*
					 * @brief	Frees page.
					 */
					void	free(char* ptr);
					
					/*
					 * @brief	Allocates page of a given size.
					 */
					char*	alloc(Paging::PageSize size);

					/*
					 * @brief	Tells buddy to leave specified part of memory alone.
					 */
					void	disablePagesPast(char* address);

					/*
					 * @brief	Is every page allocated?
					 */
					bool	isFull();
			};
	

			/*
			 * @brief	Which part of physical memory to manage?
			 */
			char*	m_regionstart;

			/*
			 * @brief	Look, Simba. Everything the light touches is our kingdom.
			 */
			size_t	m_regionsize;

			/*
			 * @brief	Buddytable with a buddy for every managed gigabyte
			 */
			Buddy*	m_gigabuddies[];

			/*
			 * @brief	Current buddycount
			 */
			size_t	m_gigabuddiesnum;

		public:
			/*
			 * @param region_start	Start of managed memory
			 * @param region_size	Size of managed memory
			 */
			BuddyAlloc(char* region_start, size_t region_size);
			
			char*	alloc(Paging::PageSize size);
			void	free(char* ptr);
	};

#endif

