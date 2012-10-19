#include "kernel.hpp"
#include "types.h"
#include "util.h"

const char* cs_bounce1="B o u n c y 1 ";
const char* cs_bounce2="B a u n c y 2 ";
char* pfb=(char*)(0xb8000);

class BallyBally
{
	protected:

		void printstr(const char* str)
		{
			kmemcpy((char*)(pfb), str, kstrlen(str));
			pfb+=80*2;
		}

//		static const char* cs_bounce1;
//		static const char* cs_bounce2;

	public:

		BallyBally()
		{
			int abc=123;
		}

		virtual void bounce()
		{
			printstr(cs_bounce1);	
		}
};

class BallyWood : public BallyBally
{
	public:
		
		virtual void bounce()
		{
			printstr(cs_bounce2);
		}
};

const char* cs_kernelgreeting="W e l c o m e   t o   K e r n e l s t e i n ! ";

BallyWood globalbally;

void kernelentry()
{
	BallyBally ball1;
	BallyWood ball2;

	ball1.bounce();
	ball2.bounce();

	static_cast<BallyBally*>(&ball2)->bounce();

	for (;;);
}

