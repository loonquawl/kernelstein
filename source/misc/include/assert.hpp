#ifndef KERN_ASSERT
#define KERN_ASSERT
#include "panic.hpp"

#define kassert(condition)		\
		if (!(condition))	\
		{			\
			kpanic((condition) && "FAILED ASSERT" && __FILE__ && __LINE__);	\
		}

#endif

#define COMPILE_ASSERT(condition)			\
	#if (condition)					\
		#error "Failed assert" ## condition	\
	#endif

