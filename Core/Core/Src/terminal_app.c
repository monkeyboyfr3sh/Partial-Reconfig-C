/*
 * partial_region.c
 *
 *  Created on: Sep 13, 2022
 *      Author: david
 */

#include "main.h"
#include <ld_script_section_A.h>
#include <stdio.h>
#include <string.h>
#include "terminal_app.h"
#include "terminal_command_strings.h"

uint8_t tx_buff[UART_BUFF_LEN] = {"This is my message!\r\n"};
uint8_t rx_buff[UART_BUFF_LEN];
uint8_t rx_byte_count = 0;
uint8_t stop_char_flag = 0;

void FUNC_1 uart_interupt_service(void)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	HAL_UART_Transmit(HUART_HANDLE, &rx_buff[rx_byte_count], 1, 10);
	rx_byte_count++;

	switch(rx_buff[rx_byte_count-1]){
		case stop_char:{
			// Reset pointer
			stop_char_flag = 1;
			uint8_t len = snprintf((char*)tx_buff,UART_BUFF_LEN,"\r\n");
			HAL_UART_Transmit(HUART_HANDLE, (uint8_t *)&tx_buff, len, 10);

			// Don't reset rx interrupt until the buffer has been handled
			break;
		}
		case backspace:{
			// Remove char
			uint8_t len = snprintf((char*)tx_buff,UART_BUFF_LEN," ");
			tx_buff[len] = backspace;
			HAL_UART_Transmit(HUART_HANDLE, (uint8_t *)&tx_buff, len+1, 10);
			rx_byte_count-= (rx_byte_count>1) ? 2 : (rx_byte_count>0) ? 1 : 0;

			// Setup to rx interrupt
			HAL_UART_Receive_IT(HUART_HANDLE, &rx_buff[rx_byte_count], 1);
			break;
		}
		default:{
			// Setup to rx interrupt
			HAL_UART_Receive_IT(HUART_HANDLE, &rx_buff[rx_byte_count], 1);
			break;
		}
	}
}


bool FUNC_2 handle_stop_char(void)
{
	// Received a stop char, so evaluate buff
	if(stop_char_flag){

		// Print buff val
		memset((char*)&tx_buff,0,UART_BUFF_LEN);
		uint8_t len = snprintf((char*)&tx_buff,UART_BUFF_LEN,">>>> ");
		memcpy(&tx_buff[len],rx_buff,rx_byte_count);
		len += snprintf((char*)&tx_buff[rx_byte_count+len],UART_BUFF_LEN,"\r\n\n");
		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, rx_byte_count+len, 10);

		// Lookup buff
		int cmd_rval = command_lookup((uint8_t *)&rx_buff,rx_byte_count);
		if(cmd_rval>-1){
			switch(cmd_rval){
			case do_math_lut:
				do_math();
				break;
			default:

			case help_lut:
				print_help();
				break;
			case clear_lut:
				clear_screen();
				break;
			case reboot_lut:
				soft_reboot();

				// If hit here, then key command detected
				break;
			}
		}

		print_shell_line();

		// Reset vars
		stop_char_flag = 0;
		rx_byte_count = 0;
		memset((char*)&rx_buff,0,UART_BUFF_LEN);
		HAL_UART_Receive_IT(&huart2, &rx_buff[rx_byte_count], 1);
		return true;
	}
	return false;
}

void print_shell_line(void)
{
	// Output terminal char
	memset((char*)&tx_buff,0,UART_BUFF_LEN);
	int len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"\r\n\e[5;92m(STM32-Shell)\e[0m $ ");
	HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
}

int SECT_A command_lookup(uint8_t *buff,uint8_t byte_count)
{
	uint8_t len = 0;
	int buff_len = strlen((uint8_t *)buff)-1;
	int cmd_len = 0;

	for(int i = 0;i<NUM_TERM_CMD;i++)
	{
		cmd_len = strlen((uint8_t *)&command_strings[i]);
		int compare_len = (buff_len>=cmd_len)? buff_len:cmd_len;
		if(!strncmp((uint8_t *)&command_strings[i],buff,compare_len)){return i;}
	}

	return -1;
}

void SECT_A command_invalid(int cmd)
{

}

void SECT_A do_math(void)
{
	uint8_t a = 0;
	uint8_t b = 0;
	int len = 0;

	for(int i = 0;i<100;i++){
		a = rand();
		b = rand();
		len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"%03d*%03d = %d\r\n",a,b,a*b);
		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
		HAL_Delay(100);
	}
	len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"\r\n",a*b,a,b);
	HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
}

void SECT_A print_help(void)
{
	int len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"List of commands:\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);

	for(int i = 0;i<NUM_TERM_CMD;i++)
	{
		len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"%s\r\n",(uint8_t *)&command_strings[i]);
		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
	}

//	cmd_len = strlen();
//	int compare_len = (buff_len>=cmd_len)? buff_len:cmd_len;
//	if(!strncmp((uint8_t *)&command_strings[i],buff,compare_len)){return i;}
}

void SECT_A clear_screen(void)
{
	memset((char*)&tx_buff,0,UART_BUFF_LEN);
	int len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"\e[2J\e[H");
	HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
}

void SECT_A soft_reboot(void)
{
	memset((char*)&tx_buff,0,UART_BUFF_LEN);
	int len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"Performing reboot in ");
	HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);

	// Count down to reboot
	for(int i = 3;i>0;i--)
	{
		memset((char*)&tx_buff,0,UART_BUFF_LEN);
		len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"%d... ",i);
		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
		HAL_Delay(1000);
	}

	// Do the reboot
	NVIC_SystemReset();
}
