#ifndef KERN_CXX_API
#define KERN_CXX_API
#include "types.h"

extern "C" {

	int __cxa_guard_acquire(uint32_t* guard_object);
	void __cxa_pure_virtual();
	int __cxa_atexit(void (*destructor)(void*), void* arg, void* dso);

};

void* operator new(size_t size);
void* operator new(size_t size, void* placement_new);
void operator delete(void* address);

extern void* __dso_handle;

void kconstruct_globals();

#endif

