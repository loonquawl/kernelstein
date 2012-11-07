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
	unsigned int prev_indent=output.get_indent();
	
	output	<< "IA-32e paging configuration\n"
		<< "---------------------------\n";
	output	<< Console::Indent(prev_indent+4);
	for (uint16_t pml4i=0; pml4i<512; ++pml4i)
	{
		PML4E* pml4_entry = PML4T+pml4i;
		if (pml4_entry->present)
		{
			output << "PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") [range: " << HexStr(512*pml4i*(unsigned long)(0x40000000)) << "-" << HexStr(512*(pml4i+1)*(unsigned long)(0x40000000)) << "]\n";
			print_PML4E(pml4_entry,output);
			output << Console::Indent(prev_indent+8);
			for (uint16_t pdpti=0; pdpti<512; ++pdpti)
			{
				uPDPTE* pdpt_entry=(uPDPTE*)(pml4_entry->physaddr*0x1000)+pdpti;

				// Every PML4E physaddr references a Page-Directory-Pointer-Table
				// or 1G Page
				if (pdpt_entry->page.is_PDPTE1G && pdpt_entry->page.present)
				{
					output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") -> 1G page\n";
					print_PDPTE1G(&pdpt_entry->page,output);
				}
				else if (pdpt_entry->pointer_table.present)
				{
					output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") -> table\n";
					print_PDPTE(&pdpt_entry->pointer_table,output);
					output << Console::Indent(prev_indent+12);
					for (uint16_t pdei=0; pdei<512; ++pdei)
					{
						uPDE* pd_entry=(uPDE*)(pdpt_entry->pointer_table.physaddr*0x1000)+pdei;

						if (pd_entry->page.is_PDE2M && pd_entry->page.present)
						{
							output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") -> 2M page\n";
							print_PDE2M(&pd_entry->page,output);
						}
						else if (pd_entry->page_table.present)
						{
							output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") -> table\n";
							print_PDE(&pd_entry->page_table,output);
							output << Console::Indent(prev_indent+16);
							for (uint16_t ptei=0; ptei<512; ++ptei)
							{
								PTE* page_table_4K=
									(PTE*)(pd_entry->page_table.physaddr*0x1000)+ptei;
								if (page_table_4K->present)
								{	
									output << " -> PML4E#" << pml4i << " (@" << HexStr((unsigned long)(pml4_entry)) << ") PDPTE#" << pdpti << " (@" << HexStr((unsigned long)(pdpt_entry)) << ") PDE#" << pdei << " (@" << HexStr((unsigned long)(pd_entry)) << ") PTE#" << ptei << " (@" << HexStr((unsigned long)(page_table_4K)) << ") -> 4K page\n";
									print_PTE(page_table_4K,output);
								}
							}
							output << Console::Indent(prev_indent+12);
						}
					}
					output << Console::Indent(prev_indent+8);
				}
			}
			output << Console::Indent(prev_indent+4);
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
		<< " " << HexStr(e->physaddr*0x1000) << "\n";
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
		<< " " << HexStr(e->physaddr*0x1000) << "\n";
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

