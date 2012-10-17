#ifndef KERN_UTIL_H
#define KERN_UTIL_H

#include "types.h"

void kmemset(void *address, char value, size_t range);
void kmemcpy(void *dest, const void *src, size_t n);

#endif

