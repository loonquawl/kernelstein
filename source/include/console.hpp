#ifndef KERN_CONSOLE
#define KERN_CONSOLE
#include "types.h"

	class Console
	{
		public:

			enum CharColor
			{
				// VGA text mode
				BLACK,
				BLUE,
				GREEN,
				CYAN,
				RED,
				MAGENTA,
				BROWN,
				LIGHTGRAY,
				DARKGRAY,
				LIGHTBLUE,
				LIGHTGREEN,
				LIGHTCYAN,
				LIGHTRED,
				LIGHTMAGENTA,
				YELLOW,
				WHITE
			};

			enum NumericalOutput
			{
				BIN,
				OCT,
				DEC,
				HEX
			};

		protected:

			static uint8_t	tabsize;

			unsigned int	cursor_x;
			unsigned int	cursor_y;

			unsigned int	width;
			unsigned int	height;

			CharColor	foreground_color;
			CharColor	background_color;
			bool		insert_blinking_text;

			NumericalOutput numerical_output_mode;

		public:

			Console()
			: cursor_x(0), cursor_y(0), width(80), height(25),
			  foreground_color(WHITE), background_color(BLACK),
			  insert_blinking_text(false),
			  numerical_output_mode(DEC)
			{}

			CharColor get_background_color()
			{
				return background_color;
			}

			CharColor get_foreground_color()
			{
				return foreground_color;
			}

			bool get_blinking()
			{
				return insert_blinking_text;
			}

			NumericalOutput get_numerical_output_mode()
			{
				return numerical_output_mode;			
			}

			void set_numerical_output_mode(NumericalOutput mode)
			{
				numerical_output_mode=mode;
			}

			virtual void print(const char* str) = 0;
			virtual void printf(const char* format, ...) = 0;

			virtual void clear()
			{
				cursor_x=cursor_y=0;
			}

			virtual void set_color(const CharColor& fg, const CharColor& bg=BLACK, bool blink=false)
			{
				foreground_color=fg;
				background_color=bg;
			}

			virtual void nextline()
			{
				++cursor_y; // implement moving lines up the screen
				cursor_x=0;
			}

			virtual void advance_cursor()
			{
				if (++cursor_x>=width)
					nextline();
			}

			virtual ~Console() {}
	};

	Console& operator<<(Console& console, const char* string);
	Console& operator<<(Console& console, long integer);
	Console& operator<<(Console& console, void* ptr);
	Console& operator<<(Console& console, Console::CharColor foreground);
	Console& operator<<(Console& console, Console::NumericalOutput mode);

	class EarlyKernelConsole : public Console
	{
		protected:

			char*	const	video_addr;
			char		colorbyte;

			char*		get_cursor_address();

		public:

			EarlyKernelConsole();

			virtual void print(const char* str);
			virtual void printf(const char* format, ...);
			virtual void clear();
			virtual void set_color(const CharColor& fg, const CharColor& bg=BLACK, bool blink=false);
			virtual void nextline();			
	};

#endif

