#ifndef KERN_ASSERT
#define KERN_ASSERT
#include "panic.hpp"

#define ASSERT(condition)		\
		if (!(condition))	\
		{			\
			kpanic((condition) && "FAILED ASSERT");	\
		}

#endif

#define COMPILE_ASSERT(condition)			\
	#if (condition)					\
		#error "Failed assert" ## condition	\
	#endif

