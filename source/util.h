#ifndef KERN_UTIL_H
#define KERN_UTIL_H

#include "types.h"

void kmemset(char *address, char value, size_t range);
void kmemcpy(char *dest, const char *src, size_t n);
unsigned int kstrlen(const char* str);

#endif

