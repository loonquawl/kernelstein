#ifndef KERN_PANIC
#define KERN_PANIC
#include "globals.hpp"
#include "types.hpp"

#define kpanic(reason)								\
	{									\
		asm volatile(							\
				"cli;						\
				 movq	%%rax,0+0x7c00;				\
				 movq	%%rbx,8+0x7c00;				\
				 movq	%%rcx,16+0x7c00;			\
				 movq	%%rdx,24+0x7c00;			\
				 movq	%%rsp,32+0x7c00;			\
				 movq	%%rbp,40+0x7c00;			\
				 movq	%%rsi,48+0x7c00;			\
				 movq	%%rdi,56+0x7c00;			\
				 leaq	(%%rip),%%rax;				\
				 movq	%%rax,64+0x7c00;			\
				 movq	%%cr3,%%rax;				\
				 movq	%%rax,72+0x7c00;			\
				 movq	%%rsp,%%rax;				\
				 movq	$0x7c00+80,%%rsp;			\
				 pushfq;					\
				 movq	%%rax, %%rsp				\
				"						\
				:						\
				:						\
				: "%rax"					\
		);								\
		g_console	<< Console::Indent(0);				\
		g_console 	<< Console::LIGHTRED						\
				<< g_shodan 							\
				<< Console::LIGHTCYAN						\
				<< "\n"								\
				<< " Kernel panic at " __FILE__ << ", line " << __LINE__ << ".\n"	\
				<< " Reason:\t" << #reason << "\n\n"				\
				<< Console::LIGHTBLUE						\
				<< Console::HEX							\
				<< " RAX:\t"  << *(uint64_t*)(0x7c00) << "\n"	\
				<< " RBX:\t"  << *(uint64_t*)(0x7c00+8) << "\n"	\
				<< " RCX:\t"  << *(uint64_t*)(0x7c00+16) << "\n"	\
				<< " RDX:\t"  << *(uint64_t*)(0x7c00+24) << "\n"	\
				<< " RSP:\t"  << *(uint64_t*)(0x7c00+32) << "\n"	\
				<< " RBP:\t"  << *(uint64_t*)(0x7c00+40) << "\n"	\
				<< " RSI:\t"  << *(uint64_t*)(0x7c00+48) << "\n"	\
				<< " RDI:\t"  << *(uint64_t*)(0x7c00+56) << "\n"	\
				<< " RIP:\t"  << *(uint64_t*)(0x7c00+64) << "\n"	\
				<< " CR3:\t"  << *(uint64_t*)(0x7c00+72) << "\n"	\
				<< " RFLAGS:\t"  << *(uint64_t*)(0x7c00+80) << "\n\n";	\
		uint8_t* stack=*(uint8_t**)(0x7c00+32);				\
		uint8_t* stack_end=stack+64;						\
		g_console << "@RSP:\n";						\
		for (; stack!=stack_end; ++stack)					\
			g_console << *stack << "\t";					\
		asm volatile("hlt");								\
	}

#endif

