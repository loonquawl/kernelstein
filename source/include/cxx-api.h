#ifndef KERN_CXX_API
#define KERN_CXX_API
#include "types.h"

extern "C" {

	int __cxa_guard_acquire(uint32_t* guard_object);
	void __cxa_pure_virtual();

};

void operator delete(void* address);

#endif
