/*
 * linker_script_sections.h
 *
 *  Created on: Oct 1, 2022
 *      Author: david
 */

#ifndef INC_LD_SCRIPT_SECTION_A_H_
#define INC_LD_SCRIPT_SECTION_A_H_

// External function call attribute definitions
#define FUNC_1 __attribute__((__section__(".function_A1")))
#define FUNC_2 __attribute__((__section__(".function_A2")))
#define FUNC_3 __attribute__((__section__(".function_A3")))

// Library attributes
#define SECT_A __attribute__((__section__(".section_A")))

#endif /* INC_LD_SCRIPT_SECTION_A_H_ */
