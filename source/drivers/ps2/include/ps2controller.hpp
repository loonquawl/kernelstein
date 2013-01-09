#ifndef KERN_PS2CONTROLLER
#define KERN_PS2CONTROLLER
#include "util.hpp"
#include "types.hpp"

// rw
#define PS2_DATA_PORT 0x60
// ro
#define PS2_STATUS_REG_PORT 0x64
// wo
#define PS2_COMMAND_REG_PORT 0x64

	class PS2Controller
	{
		public:

			struct StatusRegister
			{
				uint8_t output_ready	: 1;
				uint8_t	input_full	: 1;
				uint8_t	POST_passed	: 1;
				uint8_t	command_data	: 1;
				uint8_t	unknown1	: 1;
				uint8_t	unknown2	: 1;
				uint8_t	timeout_err	: 1;
				uint8_t	parity_err	: 1;
			} __attribute__((packed));

		protected:
			
			PS2Controller() {}

		public:
			static void read_status_register(StatusRegister& out);

			
	};

#endif

