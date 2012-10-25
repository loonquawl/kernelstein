#include <stdarg.h>
#include "console.hpp"
#include "string.hpp"
#include "util.h"

EarlyKernelConsole::EarlyKernelConsole()
: video_addr((char*)(0xb8000))
{
	// calculate colorbyte
	set_color(foreground_color, background_color, insert_blinking_text);
	clear();
}

void EarlyKernelConsole::print(const char* str)
{
	char* console=video_addr+cursor_y*width*2+cursor_x*2;
	while (*str)
	{
		if (*str=='\n')
		{
			nextline();
			++str;
		}
		else
		{
			*console++=*str++;
			*console++=colorbyte;
			if (++cursor_x>=width)
				nextline();
		}
	}
}

void EarlyKernelConsole::printf(const char* format, ...)
{
	char buffer[4096];
	va_list v;
	va_start(v,format);
	kvsprintf(buffer,format,v);
	print(buffer);
	va_end(v);
}

void EarlyKernelConsole::clear()
{
	Console::clear();
	for (char* ptr=video_addr; ptr<video_addr+width*height*2;)
	{
		*ptr++=' ';
		*ptr++=colorbyte;
	}
}

void EarlyKernelConsole::nextline()
{
	Console::nextline();
	if (cursor_y>=height)
	{
		// copy lines upward
		kmemcpy(video_addr,video_addr+width*2,width*2*(height-1));
		// clear last line
		for (char* ptr=video_addr+width*2*(height-1); ptr<video_addr+width*height*2;)
		{
			*ptr++=' ';
			*ptr++=colorbyte;
		}			
		cursor_y=height-1;
	}
}

void EarlyKernelConsole::set_color(const CharColor& fg, const CharColor& bg, bool blink)
{
	Console::set_color(fg,bg);
	colorbyte=(char)((int)(fg)|(int)(bg)<<4|(int)(blink)<<7);
}

