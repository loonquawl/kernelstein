#ifndef KERN_LIST
#define KERN_LIST
#include <nullptr.h>

	template<typename Payload>
	class List<Payload>
	{
		protected:
			List<Payload> *prev;
			List<Payload> *next;
			
			Payload	value;
		public:
			List()
			: prev(NULL), next(NULL)
			{}

			List<Payload>& prev()
			{
				return prev;
			}

			List<Payload>& next()
			{
				return next;
			}

			Payload& operator*()
			{
				return value;
			}

			const Payload& operator*() const
			{
				return value;
			}

			List<Payload>& operator+(int pos)
			{
				List<Payload> *ret=this;
				while(pos && ret!=NULL)
				{
					if (pos<0)
						ret=ret->prev();
					else
						ret=ret->next();
				}
				return ret;
			}
	};

#endif

