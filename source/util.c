#include "util.h"

void kmemset(char *address, char value, size_t range)
{
	char *pend=address+range;
	for (; address<pend; *address++ = value);
}

void kmemcpy(char *dest, const char *src, size_t n)
{
	for(; n; --n) *dest++ = *src++;
}

