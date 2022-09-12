/*
 * uart_interface.h
 *
 *  Created on: Aug 16, 2022
 *      Author: david
 */

#ifndef INC_UART_INTERFACE_H_
#define INC_UART_INTERFACE_H_
#include <stdbool.h>
#include <stdint.h>
#include "partial_region.h"

typedef enum {
	stop_char = '\r',
	backspace = 8
}special_char_t;
#define UART_BUFF_LEN	256
#define HUART_HANDLE	(&huart2)

extern uint8_t tx_buff[UART_BUFF_LEN];
extern uint8_t rx_buff[UART_BUFF_LEN];
extern uint8_t rx_byte_count;
extern uint8_t stop_char_flag;

#endif /* INC_UART_INTERFACE_H_ */
