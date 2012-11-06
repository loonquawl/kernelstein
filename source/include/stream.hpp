#ifndef KERN_STREAM
#define KERN_STREAM

	class OStream
	{
		public:

			virtual OStream& operator<<(const char* string) = 0;
			virtual OStream& operator<<(long integer) = 0;
			virtual OStream& operator<<(void* ptr) = 0;
	};

	class IStream
	{
		public:

			virtual IStream& operator>>(const char* string) = 0;
			virtual IStream& operator>>(long integer) = 0;
			virtual IStream& operator>>(void* ptr) = 0;
	};

	class IOStream : public OStream, public IStream
	{
		
	};

#endif

