#include "cxx-api.h"

extern void* constructor_table;
extern void* constructor_table_end;

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

int __cxa_atexit(void (*destructor)(void*), void* arg, void* dso)
{
	return 0;
}

void* operator new(size_t size)
{
}

void* operator new(size_t size, void* placement_new)
{
}

void operator delete(void* address)
{
}

void* __dso_handle=0;

void kconstruct_globals()
{
	typedef void (**fptr)();
	fptr constructor = (fptr)(&constructor_table);
	while (constructor<(fptr)(&constructor_table_end))
	{
		(*constructor)();
		constructor+=sizeof(uint64_t);
	}
}

