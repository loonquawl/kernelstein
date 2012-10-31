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

void koutb(char port, char value)
{
	asm volatile ("out %%al, %%dx"::"a" (value), "d" (port));
}

char kinb(char port)
{
	char ret=0;
	asm volatile ("inb %%dx, %%al":"=a" (ret):"d" (port));
	return ret;
}

