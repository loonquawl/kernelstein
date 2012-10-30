#ifndef KERN_STRING
#define KERN_STRING
#include <stdarg.h>
#include "types.h"
	
	size_t	kstrlen(const char* str);
	char*	kstrcpy(char* dest, const char* src);
	int	ksprintf(char* str, const char* fmt, ...);
	int	kvsprintf(char* str, const char* fmt, va_list v);
	int	kitoa(long value, char* dest, long base);

#endif

