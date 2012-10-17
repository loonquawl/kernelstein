
void kernelentry()
{

	*(char*)(0xb8000)='G';
	memcpy((void*)(0xb8000), (void*)(cs_kernelgreeting), strlen(cs_kernelgreeting));
	*(char*)(0xb8000)='M';
	for (;;);
}

void *memcpy(void *dest, const void *src, size_t n)
{
	for (; n; --n) *(char*)(dest++) = *(char*)(src++);
	return dest;
}

size_t strlen(const char* str)
{
	size_t len=0;
	for (; *str++; ++len);
	return len;
}

const char* cs_kernelgreeting="W e l c o m e   t o   K e r n e l s t e i n ! ";

