#ifndef __PARTIAL_REGION_H__
#define __PARTIAL_REGION_H__
#include <usart.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	stop_char = '\r',
	backspace = 8
}special_char_t;

#define UART_BUFF_LEN	2048
#define HUART_HANDLE	(&huart2)

extern uint8_t tx_buff[UART_BUFF_LEN];
extern uint8_t rx_buff[UART_BUFF_LEN];
extern uint8_t rx_byte_count;
extern uint8_t stop_char_flag;

// external functions
void uart_interupt_service(void);
bool handle_stop_char(void);
void print_shell_line(void);

// internal functions
int command_lookup(uint8_t *buff,uint8_t byte_count);
void do_math(void);
void print_help(void);

#endif
