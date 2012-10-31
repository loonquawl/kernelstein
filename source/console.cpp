#include <stdarg.h>
#include "console.hpp"
#include "string.hpp"
#include "util.h"

uint8_t Console::tabsize = 5;

EarlyKernelConsole::EarlyKernelConsole()
: video_addr((char*)(0xb8000))
{
	// calculate colorbyte
	set_color(foreground_color, background_color, insert_blinking_text);
	clear();
}

char* EarlyKernelConsole::get_cursor_address()
{
	return video_addr+cursor_y*width*2+cursor_x*2;
}

void EarlyKernelConsole::print(const char* str)
{
	char* console=get_cursor_address();
	while (*str)
	{
		if (*str=='\n')
		{
			nextline();
			console=get_cursor_address();
		}
		else if (*str=='\t')
		{
			uint8_t i=cursor_x%tabsize;
			for (; i<tabsize; ++i)
			{				
				*console++=' ';
				*console++=colorbyte;
				advance_cursor();
				console=get_cursor_address();
			}
		}
		else
		{
			*console++=*str;
			*console++=colorbyte;
			advance_cursor();
		}
		++str;
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


Console& operator<<(Console& console, const char* string)
{
	console.print(string);
	return console;
}

Console& operator<<(Console& console, void* ptr)
{
	return operator<<(console,(unsigned long)(ptr));
}

Console& operator<<(Console& console, long integer)
{
	const char* fmt;
	switch (console.get_numerical_output_mode())
	{
		case Console::BIN:
			fmt="%b";
			break;
		case Console::OCT:
			fmt="%o";
			break;
		case Console::HEX:
			fmt="%h";
			break;
		case Console::DEC:
			fmt="%d";
			break;
		default:
			fmt="";
			break;
	}
	console.printf(fmt,integer);
	return console;
}

Console& operator<<(Console& console, Console::CharColor foreground)
{
	console.set_color(foreground,console.get_background_color(),console.get_blinking());
	return console;
}

Console& operator<<(Console& console, Console::NumericalOutput mode)
{
	console.set_numerical_output_mode(mode);
	return console;
}

