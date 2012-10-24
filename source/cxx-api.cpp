#include "cxx-api.h"

int __cxa_guard_acquire(uint32_t* guard_object)
{
	if (!*guard_object)
		return *guard_object=1;
	return 0;
}

