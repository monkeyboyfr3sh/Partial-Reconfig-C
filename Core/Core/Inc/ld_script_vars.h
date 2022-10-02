/*
 * exposed_partial_region.h
 *
 *  Created on: Oct 1, 2022
 *      Author: david
 */

#ifndef INC_LD_SCRIPT_VARS_H_
#define INC_LD_SCRIPT_VARS_H_

// ############### Section 1 Code ###############
extern char some_var[] = "some_var";
extern char lut_func_ptr_A1;
extern void (*function_A1)(uint32_t) = 0;

// ############### Section 2 Code ###############
extern char another_var[] = "another_var";
extern char lut_func_ptr_A2;
extern bool (*function_A2)(void) = 0;

#endif /* INC_LD_SCRIPT_VARS_H_ */
