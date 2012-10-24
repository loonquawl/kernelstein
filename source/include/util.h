#ifndef KERN_UTIL
#define KERN_UTIL
#include "types.h"

void kmemset(char *address, char value, size_t range);
void kmemcpy(char *dest, const char *src, size_t n);

#endif

