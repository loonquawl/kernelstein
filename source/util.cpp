#include "util.hpp"

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
	asm volatile ("outb %%al, %%dx"::"a" (value), "d" (port));
}

char kinb(char port)
{
	char ret=0;
	asm volatile ("inb %%dx, %%al":"=a" (ret):"d" (port));
	return ret;
}

void kclihlt()
{
	asm volatile ("cli; hlt;");
}

void khexout(const char* data, size_t length, Console& output)
{
	const char* edata=data+length;
	output << Console::HEX;
	for (; data!=edata; ++data)
		output << *data << "\t";
	output << "\n";
}

