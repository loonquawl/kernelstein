#include "cxx-api.h"

int __cxa_guard_acquire(uint32_t* guard_object)
{
	*(char*)(0xb8000+20*2)='Q';
	if (!*guard_object)
		return *guard_object=1;
	return 0;
}

void __cxa_pure_virtual()
{
}

void operator delete(void* address)
{
}

