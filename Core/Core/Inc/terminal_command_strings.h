/*
 * terminal_command_strings.h
 *
 *  Created on: Oct 1, 2022
 *      Author: david
 */

#ifndef INC_TERMINAL_COMMAND_STRINGS_H_
#define INC_TERMINAL_COMMAND_STRINGS_H_

#define NUM_TERM_CMD		(4)
#define TERM_CMD_LEN_LIM	(256)
uint8_t command_strings[NUM_TERM_CMD][TERM_CMD_LEN_LIM] = {
		"do_math",
		"help",
		"clear",
		"reboot",
};

typedef enum {
	do_math_lut,
	help_lut,
	clear_lut,
	reboot_lut,
}cmd_enum;

#endif /* INC_TERMINAL_COMMAND_STRINGS_H_ */
