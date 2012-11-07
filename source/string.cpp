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
			union
			{
				long d;
				unsigned long u;
				char c, *s;
			};

			switch (*(fmt+1))
			{
				case 's':
					s = va_arg(v, char*);
					kstrcpy(strpos, s);
					strpos+=kstrlen(s);
					fmt+=2;
					break;
				case 'd':
					d = va_arg(v, long);
					strpos+=kitoa(d,strpos,10);
					fmt+=2;
					break;
				case 'u':
					u = va_arg(v, unsigned long);
					strpos+=kitoa(u,strpos,10);
					fmt+=2;
					break;
				case 'o':
					u = va_arg(v, unsigned long);
					strpos+=kitoa(u,strpos,8);
					fmt+=2;
					break;
				case 'h':
					u = va_arg(v, unsigned long);
					*strpos++='0';
					*strpos++='x';
					strpos+=kitoa(u,strpos,16);
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

