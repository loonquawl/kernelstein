#ifndef KERN_UTIL
#define KERN_UTIL
#include "types.hpp"
#include "console.hpp"

	#define PRINTVAR(console,var) (console << __FILE__ << ":" << __LINE__ << " " << #var << "=" << (var) << "\n")
	#define ELEMENTS(array) (sizeof(array)/sizeof(*array))
	#define BITS(var) (sizeof(var)*8)

	void kmemset(char *address, char value, size_t range);
	void kmemcpy(char *dest, const char *src, size_t n);
	void koutb(char port, char value);
	char kinb(char port);
	void kclihlt();
	void khexout(const char* data, size_t length, Console& output);

#endif

