#ifndef KERN_UTIL
#define KERN_UTIL
#include "types.h"

#define PRINTVAR(console,var) (console << __FILE__ << ":" << __LINE__ << " " << #var << "=" << (var) << "\n")

void kmemset(char *address, char value, size_t range);
void kmemcpy(char *dest, const char *src, size_t n);
void koutb(char port, char value);
char kinb(char port);
void kclihlt();

#endif

