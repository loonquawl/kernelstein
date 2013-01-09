#include "ps2controller.hpp"
#include "util.hpp"

void PS2Controller::read_status_register(PS2Controller::StatusRegister& output)
{
	*(char*)(&output) = kinb(PS2_STATUS_REG_PORT);
}

