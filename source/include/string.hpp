#ifndef KERN_STRING
#define KERN_STRING
#include <stdarg.h>
#include "types.hpp"
#include "math.hpp"
	
	size_t	kstrlen(const char* str);
	char*	kstrcpy(char* dest, const char* src);
	int	ksprintf(char* str, const char* fmt, ...);
	int	kvsprintf(char* str, const char* fmt, va_list v);

	class HexStr
	{
		// bytes*2 nibbles + 0,x,\0
		char string[sizeof(unsigned long)*2+3];

		public:
			
			HexStr(unsigned long value)
			{
				ksprintf(string,"%h",value);
			}

			operator const char*() const
			{
				return string;
			}
	};

	template<typename ValueType, typename BaseType>
	int kitoa(ValueType value, char* dest, BaseType base)
	{
		int digits=-1;

		if (!base)
			return 0;
		if (!value)
		{
			*dest='0';
			return 1;
		}
		if (value<0)
			*dest++='-';
		// it's either this or log10 on FPU or i'm dumb ^^
		for (ValueType i=value; i; i/=base, ++digits);
		dest+=digits;
		for (; value; value/=base)
		{
			char digit=abs(value)%base;
			*dest-- = ((digit<10) ? '0' : 'a'-10) + digit;
		}
		return digits + (value<0) + 1;
	}

#endif

