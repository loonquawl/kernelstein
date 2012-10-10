typedef unsigned int size_t;
const char* cs_kernelgreeting;
void *memcpy(void* dest, const void *src, size_t n);
size_t strlen(const char* str);

void kernelentry()
{
//	memcpy((void*)(0xb8000), (void*)(cs_kernelgreeting), strlen(cs_kernelgreeting));
	for (;;);
}

void *memcpy(void *dest, const void *src, size_t n)
{
	for (; n; --n) *(char*)(dest++) = *(char*)(src++);
}

size_t strlen(const char* str)
{
	size_t len=0;
	for (; *str; ++len);
	return len;
}

const char* cs_kernelgreeting="Welcome to Kernelstein!";

