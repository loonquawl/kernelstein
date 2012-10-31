#include "kernel.hpp"
#include "string.hpp"
#include "util.h"
#include "cxx-api.h"
#include "globals.hpp"
#include "panic.hpp"

void kernelentry()
{
	kconstruct_globals();

	g_console << "Awyeah!" << "\n";


/*
	char ps2commport=0x64;
	char ps2dataport=0x60;
	char outbyte=0xd0; // test controller
	char inbyte=0; // should be PS2 controller output port

	koutb(ps2commport,outbyte);
	inbyte=kinb(ps2dataport+1);
*/
	for (;;);
}

