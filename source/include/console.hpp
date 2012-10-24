#ifndef KERN_CONSOLE
#define KERN_CONSOLE

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

		protected:

			unsigned int	cursor_x;
			unsigned int	cursor_y;

			unsigned int	width;
			unsigned int	height;

			CharColor	foreground_color;
			CharColor	background_color;
			bool		insert_blinking_text;

		public:

			Console()
			: cursor_x(0), cursor_y(0), width(80), height(25),
			  foreground_color(WHITE), background_color(BLACK),
			  insert_blinking_text(false)
			{}

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

			virtual ~Console() {}
	};

	class EarlyKernelConsole : public Console
	{
		protected:

			char*	const	video_addr;
			char		colorbyte;

		public:

			EarlyKernelConsole();

			virtual void print(const char* str);
			virtual void printf(const char* format, ...);
			virtual void clear();
			virtual void set_color(const CharColor& fg, const CharColor& bg=BLACK, bool blink=false);
			virtual void nextline();
	};

#endif

