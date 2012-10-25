#include <stdarg.h>
#include "string.hpp"
#include "math.hpp"

size_t kstrlen(const char* str)
{
	const char* start=str;
	for (; *str; ++str);
	return str-start;
}

char* kstrcpy(char* dest, const char* src)
{
	for (char* ptr=dest; *src; *ptr++=*src++);
	return dest;
}

int ksprintf(char* str, const char* fmt, ...)
{
	va_list v;
	va_start(v,fmt);
	int ret = kvsprintf(str, fmt, v);
	va_end(v);
	return ret;
}

int kvsprintf(char* str, const char* fmt, va_list v)
{
	char* strpos=str;

	while (*fmt)
	{
		if (*fmt=='\\' && *(fmt+1)=='%')
		{
			*strpos++=*fmt++;
			*strpos++=*fmt++;
		}
		else if (*fmt=='%')
		{
			int d;
			char c, *s;

			switch (*(fmt+1))
			{
				case 's':
					s = va_arg(v, char*);
					kstrcpy(strpos, s);
					strpos+=kstrlen(s);
					fmt+=2;
					break;
				case 'd':
					d = va_arg(v, int);
					strpos+=kitoa(d,strpos,10);
					fmt+=2;
					break;
				case 'o':
					d = va_arg(v, int);
					strpos+=kitoa(d,strpos,8);
					fmt+=2;
					break;
				case 'h':
					d = va_arg(v, int);
					*strpos++='0';
					*strpos++='x';
					strpos+=kitoa(d,strpos,16);
					fmt+=2;
					break;
				case 0:
					++fmt;
					break;
				default:
					*strpos++=*fmt++;
					*strpos++=*fmt++;
					break;
			}
		}
		else
		{
			*strpos++=*fmt++;
		}
	}
	*strpos++=0;
	return strpos-str;
}

int kitoa(int value, char* dest, int base)
{
	// time to get a reputation for writing messy code!

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
	for (int i=value; i; i/=base, ++digits);
	dest+=digits;
	for (; value; value/=base)
	{
		char digit=abs(value)%base;
		*dest-- = ((digit<10) ? '0' : 'a'-10) + digit;
	}
	return digits + (value<0) + 1;
}

