/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include "uart_interface.h"
#include "partial_region.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// External function call attribute definitions
#define FUNC_1 __attribute__((__section__(".function_1")))
#define FUNC_2 __attribute__((__section__(".function_2")))

// Library attributes
#define SECT_1 __attribute__((__section__(".section_1")))
#define SECT_2 __attribute__((__section__(".section_2")))

// ############### Section 1 Code ###############
extern char some_var[] = "some_var";
extern char lut_func_ptr_1;
extern void (*functoin_1)(uint32_t) = 0;

// ############### Section 2 Code ###############
extern char another_var[] = "another_var";
extern char lut_func_ptr_2;
extern void (*functoin_2)(uint32_t) = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void FUNC_1 uart_interupt_service(void);
bool FUNC_2 handle_stop_char(void);

void FUNC_1 uart_interupt_service(void)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	HAL_UART_Transmit(HUART_HANDLE, &rx_buff[rx_byte_count], 1, 10);
	rx_byte_count++;

	switch(rx_buff[rx_byte_count-1]){
		case stop_char:{
			stop_char_flag = 1;
			uint8_t len = snprintf((char*)tx_buff,UART_BUFF_LEN,"\r\n");
			HAL_UART_Transmit(HUART_HANDLE, (uint8_t *)&tx_buff, len, 10);
			break;
		}
		case backspace:{
			uint8_t len = snprintf((char*)tx_buff,UART_BUFF_LEN," ");
			tx_buff[len] = backspace;
			HAL_UART_Transmit(HUART_HANDLE, (uint8_t *)&tx_buff, len+1, 10);
			rx_byte_count-=2;
			HAL_UART_Receive_IT(HUART_HANDLE, &rx_buff[rx_byte_count], 1);
			break;
		}
		default:{
			HAL_UART_Receive_IT(HUART_HANDLE, &rx_buff[rx_byte_count], 1);
			break;
		}
	}
}


bool FUNC_2 handle_stop_char(void)
{
	if(stop_char_flag){
	  uint8_t len = snprintf((char*)&tx_buff,UART_BUFF_LEN,">>>> ");
	  memcpy(&tx_buff[len],rx_buff,rx_byte_count);
	  len += snprintf((char*)&tx_buff[rx_byte_count+len],UART_BUFF_LEN,"\r\n\n$ ");
	  HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, rx_byte_count+len, 10);

	  stop_char_flag = 0;
	  rx_byte_count = 0;
	  HAL_UART_Receive_IT(&huart2, &rx_buff[rx_byte_count], 1);
	  return true;
	}
	return false;
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* Section 1 Code (some) */
//void FUNC_1 some_operation(uint32_t dlyTicks)
//{
//	uint32_t tick = HAL_GetTick();
//	while(HAL_GetTick()-tick < dlyTicks);
//	uint8_t tx_buff[1024];
//	uint8_t val = 222;
//	uint8_t len = snprintf((char*)&tx_buff,1024,"Some other words %d\r\n",val);
//
//	for(int i = 0;i<15;i++){
//		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
//		HAL_Delay(150);
//	}
//}
//
///* Section 2 Code (another) */
//void FUNC_2 another_operation(uint32_t dlyTicks)
//{
//	uint32_t tick = HAL_GetTick();
//	while(HAL_GetTick()-tick < dlyTicks);
//	uint8_t tx_buff[1024];
//	uint8_t val = 22;
//	uint8_t len = snprintf((char*)&tx_buff,1024,"Another word %d\r\n",val);
//
//	for(int i = 0;i<15;i++){
//		HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
//		HAL_Delay(100);
//	}
//}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t tx_buff[UART_BUFF_LEN] = {"This is my message!\r\n"};
uint8_t rx_buff[UART_BUFF_LEN];
uint8_t rx_byte_count = 0;
uint8_t stop_char_flag = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  uint8_t tx_buff[1024];
  uint8_t len;

  // Entry
  len = snprintf((char*)&tx_buff,1024,"\r\n < ==================== Fixed Core Entry point ==================== > \r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);

  // Setup terminal
  rx_byte_count = 0;
  HAL_UART_Receive_IT(HUART_HANDLE, &rx_buff[rx_byte_count], 1);

  len = snprintf((char*)&tx_buff,UART_BUFF_LEN,"\r\n\n$ ");
  HAL_UART_Transmit(HUART_HANDLE, (uint8_t *)&tx_buff, rx_byte_count+len, 10);

  // Partial reconfig code
  void (*real_functoin_1)(uint32_t) = &uart_interupt_service;
  void (*real_functoin_2)(uint32_t) = &handle_stop_char;

  functoin_1 = &lut_func_ptr_1+1;
  functoin_2 = &lut_func_ptr_2+1;

  if(functoin_1!=real_functoin_1){
	  uint8_t len = snprintf((char*)&tx_buff,1024,"INCORRECT FUNC 1 PTR!!!\r\n");
	  while(1){
		  HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
	  }
  }
  if(functoin_2!=real_functoin_2){
	  uint8_t len = snprintf((char*)&tx_buff,1024,"INCORRECT FUNC 2 PTR!!!\r\n");
	  while(1){
		  HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);
	  }
  }

  functoin_1(500);
  functoin_2(500);

//  some_operation(500);
//  another_operation(200);

  uint32_t tick = HAL_GetTick();
  uint32_t data = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  bool rval = handle_stop_char();
    /* USER CODE BEGIN 3 */
  }

  len = snprintf((char*)&tx_buff,1024," < ==================== Fixed Core Exit point ==================== > \r\n");
  HAL_UART_Transmit(&huart2, (uint8_t *)&tx_buff, len, 10);

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		uart_interupt_service();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
