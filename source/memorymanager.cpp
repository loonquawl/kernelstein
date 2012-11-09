#include "memorymanager.hpp"
#include "string.hpp"
#include "nullptr.h"
#include "types.hpp"
#include "panic.hpp"
#include "util.hpp"

using namespace BIOS;
using namespace Paging;

MemoryMapEntry* const MemoryManager::BIOS_memmap_addr = (MemoryMapEntry*)(0x500);
PML4E* MemoryManager::PML4T = (PML4E*)(0x1000);

void* MemoryManager::get_page(MemoryManager::PageSize size)
{

}

void MemoryManager::print_pagetree(Console& output)
{
	const unsigned long gigabyte=0x40000000;
	const unsigned long twomegs=0x200000;
	const unsigned long fourkb=0x1000;
	unsigned int prev_indent=output.get_indent();
	
	output	<< "IA-32e paging configuration\n"
		<< "---------------------------\n";
	/*
	 *	output	<< "Sanity check:\n";
	 *	PRINTVAR(output,sizeof(PML4E));
	 *	PRINTVAR(output,sizeof(PDPTE));
	 *	PRINTVAR(output,sizeof(PDPTE1G));
	 *	PRINTVAR(output,sizeof(PDE));
	 *	PRINTVAR(output,sizeof(PDE2M));
	 *	PRINTVAR(output,sizeof(PTE));
	 *	output << "\n";
	 */
	output	<< Console::Indent(prev_indent+2);
	for (uint16_t pml4i=0; pml4i<512; ++pml4i)
	{
		PML4E* pml4_entry = PML4T+pml4i;
		if (pml4_entry->present)
		{
			unsigned long pml4e_range_start=512*pml4i*gigabyte;		// PML4E maps up to 512GB
			unsigned long pml4e_range_end=512*(pml4i+1)*gigabyte;

			output << "PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") [range: " << HexStr(pml4e_range_start) << "-" << HexStr(pml4e_range_end) << "]\n";
			print_PML4E(pml4_entry,output);
			output << Console::Indent(prev_indent+4);
			for (uint16_t pdpti=0; pdpti<512; ++pdpti)
			{
				unsigned long pdpte_range_start=pml4e_range_start + pdpti*gigabyte;
				unsigned long pdpte_range_end=pdpte_range_start + gigabyte;
				uPDPTE* pdpt_entry=(uPDPTE*)(pml4_entry->physaddr*0x1000)+pdpti;

				// Every PML4E physaddr references a Page-Directory-Pointer-Table
				// or 1G Page
				if (pdpt_entry->page.is_PDPTE1G && pdpt_entry->page.present)
				{
					output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") -> 1G page\n";
					output << " Which maps [" << HexStr(pdpte_range_start) << "-" << HexStr(pdpte_range_end) << "] to [" << HexStr(pdpt_entry->page.physaddr) << "-" << HexStr(pdpt_entry->page.physaddr+gigabyte) << "]\n";
					print_PDPTE1G(&pdpt_entry->page,output);
				}
				else if (pdpt_entry->pointer_table.present)
				{
					output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") -> table\n";
					print_PDPTE(&pdpt_entry->pointer_table,output);
					output << Console::Indent(prev_indent+6);
					for (uint16_t pdei=0; pdei<512; ++pdei)
					{
						unsigned long pde_range_start=pdpte_range_start + pdei*twomegs;
						unsigned long pde_range_end=pde_range_start + twomegs;
						uPDE* pd_entry=(uPDE*)(pdpt_entry->pointer_table.physaddr*0x1000)+pdei;

						if (pd_entry->page.is_PDE2M && pd_entry->page.present)
						{
							output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") -> 2M page\n";
							output << " Which maps [" << HexStr(pde_range_start) << "-" << HexStr(pde_range_end) << "] to [" << HexStr(pd_entry->page.physaddr) << "-" << HexStr(pd_entry->page.physaddr+twomegs) << "]\n";
							print_PDE2M(&pd_entry->page,output);
						}
						else if (pd_entry->page_table.present)
						{
							output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") -> table\n";
							print_PDE(&pd_entry->page_table,output);
							output << Console::Indent(prev_indent+8);
							for (uint16_t ptei=0; ptei<512; ++ptei)
							{
								unsigned long pte_range_start=pde_range_start + ptei*fourkb;
								unsigned long pte_range_end=pte_range_start + fourkb;
								PTE* page_table_4K=
									(PTE*)(pd_entry->page_table.physaddr*0x1000)+ptei;

								if (page_table_4K->present)
								{	
									output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") PTE#" << ptei << " (@" << HexStr((unsigned long)(page_table_4K)) << ") -> 4K page\n";
									output << " Which maps [" << HexStr(pte_range_start) << "-" << HexStr(pte_range_end) << "] to [" << HexStr(page_table_4K->physaddr*0x1000) << "-" << HexStr(page_table_4K->physaddr*0x1000+fourkb) << "]\n";
									print_PTE(page_table_4K,output);
								}
							}
							output << Console::Indent(prev_indent+6);
						}
					}
					output << Console::Indent(prev_indent+4);
				}
			}
			output << Console::Indent(prev_indent+2);
		}
	}
	output << Console::Indent(prev_indent);
}

void MemoryManager::print_PML4E(PML4E* e, OStream& output)
{
	output	<< " P W S WT CD A XD PA\n"
		<< " " << e->present
		<< " " << e->rw
		<< " " << e->supervisor
		<< "  " << e->write_through
		<< "  " << e->cache_disable
		<< " " << e->accessed
		<< "  " << e->execute_disable
		<< " " << HexStr(e->physaddr*0x1000) << "\n";
}

void MemoryManager::print_PDPTE(Paging::PDPTE* e, OStream& output)
{
	// same stuff
	print_PML4E((PML4E*)(e),output);
}

void MemoryManager::print_PDPTE1G(Paging::PDPTE1G* e, OStream& output)
{
	output	<< " P W S WT CD A D PS G PAT XD PA\n"
		<< " " << e->present
		<< " " << e->rw
		<< " " << e->supervisor
		<< "  " << e->write_through
		<< "  " << e->cache_disable
		<< " " << e->accessed
		<< " " << e->dirty
		<< "  " << e->is_PDPTE1G
		<< " " << e->global
		<< "   " << e->PAT
		<< "  " << e->execute_disable
		<< " " << HexStr(e->physaddr) << "\n";
}

void MemoryManager::print_PDE(Paging::PDE* e, OStream& output)
{
	print_PML4E((PML4E*)(e),output);
}

void MemoryManager::print_PDE2M(Paging::PDE2M* e, OStream& output)
{
	output	<< " P W S WT CD A D PS G PAT XD PA\n"
		<< " " << e->present
		<< " " << e->rw
		<< " " << e->supervisor
		<< "  " << e->write_through
		<< "  " << e->cache_disable
		<< " " << e->accessed
		<< " " << e->dirty
		<< "  " << e->is_PDE2M
		<< " " << e->global
		<< "   " << e->PAT
		<< "  " << e->execute_disable
		<< " " << HexStr(e->physaddr) << "\n";
}

void MemoryManager::print_PTE(Paging::PTE* e, OStream& output)
{
	output	<< " P W S WT CD A D PAT G XD PA\n"
		<< " " << e->present
		<< " " << e->rw
		<< " " << e->supervisor
		<< "  " << e->write_through
		<< "  " << e->cache_disable
		<< " " << e->accessed
		<< " " << e->dirty
		<< "   " << e->PAT
		<< " " << e->global
		<< "  " << e->execute_disable
		<< " " << HexStr(e->physaddr*0x1000) << "\n";
}

