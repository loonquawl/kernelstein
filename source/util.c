#include "util.h"

void kmemset(void *address, char value, size_t range)
{
	char *pend=address+range;
	for (; address<pend; *address++ = value);
}

void kmemcpy(void *dest, const void *src, size_t n)
{
	for(; n; --n) *dest++ = *src++;
}

