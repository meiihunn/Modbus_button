/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define COL		GPIOA
#define COL1	GPIO_PIN_3
#define COL2	GPIO_PIN_4
#define COL3	GPIO_PIN_6
#define COL4	GPIO_PIN_7
#define ROW		GPIOA
#define ROW1	GPIO_PIN_0
#define ROW2	GPIO_PIN_1
#define ROW3	GPIO_PIN_2

int a[3][4]={
		{'1','2','3','4'},
		{'5','6','7','8'},
		{'9','#','0','*'},
};
char scan_keypad();
volatile uint8_t g_row_flag =0;
volatile uint8_t data_received_flag=0;
char key='\0';
char last_key ='\0';

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	HAL_Delay(10);
	if(GPIO_Pin == GPIO_PIN_0){
		g_row_flag=1;
		key = scan_keypad();
	}
	else if(GPIO_Pin == GPIO_PIN_1)
	{
		g_row_flag=2;
		key = scan_keypad();
	}
	else if (GPIO_Pin == GPIO_PIN_2)
	{
		g_row_flag=3;
		key = scan_keypad();
	}
}



char scan_keypad(void){
	key='\0';
	int selected_col=0;
	for (int col =0; col <4; col++){
//		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
//		HAL_NVIC_DisableIRQ(EXTI1_IRQn);
//		HAL_NVIC_DisableIRQ(EXTI2_IRQn);
// tat ngat -> bat HAL_GPIO_writepin COL...
//		HAL_GPIO_WritePin(COL, COL1 | COL2 | COL3 | COL4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL, COL1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL, COL2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL, COL3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(COL, COL4, GPIO_PIN_SET);

		if(col == 0) HAL_GPIO_WritePin(COL, COL1, GPIO_PIN_RESET);
		if(col == 1) HAL_GPIO_WritePin(COL, COL2, GPIO_PIN_RESET);
		if(col == 2) HAL_GPIO_WritePin(COL, COL3, GPIO_PIN_RESET);
		if(col == 3) HAL_GPIO_WritePin(COL, COL4, GPIO_PIN_RESET);

//		if( ( GPIO_PIN_0)==0){
//			selected_col = col;
//			key = a[0][selected_col];
//		}
//		else if( (GPIO_PIN_1)==0){
//			selected_col = col;
//			key = a[0][selected_col];
//		}
//		else if( ( GPIO_PIN_2)==0){
//			selected_col = col;
//			key = a[0][selected_col];
//		}

		if (/*g_row_flag==1 &&*/(HAL_GPIO_ReadPin(ROW, ROW1) == GPIO_PIN_RESET)){
			selected_col = col;
			key = a[0][selected_col];
		}
		else if(/*g_row_flag==2 &&*/(HAL_GPIO_ReadPin(ROW, ROW2) == GPIO_PIN_RESET)){
			selected_col = col;
			key = a[1][selected_col];
		}
		else if(/*g_row_flag==3 && */(HAL_GPIO_ReadPin(ROW, ROW3) == GPIO_PIN_RESET)){
			selected_col = col;
			key = a[2][selected_col];
		}

		if(key != '\0') break;
	}



	HAL_GPIO_WritePin(COL, COL1, 0);
	HAL_GPIO_WritePin(COL, COL2, 0);
	HAL_GPIO_WritePin(COL, COL3, 0);
	HAL_GPIO_WritePin(COL, COL4, 0);

//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	// bat ngat (co ham enable interrupt ex:   HAL_NVIC_EnableIRQ(EXTI2_IRQn);)
	g_row_flag = 0;
	return key;

}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rx_data;
uint8_t tx_data[6]="hello\n";
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		HAL_UART_Transmit(&huart1,&rx_data,sizeof(rx_data), 100);
		HAL_UART_Receive_IT(&huart1, &rx_data, 1);
	}
}

void display_key(char key) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&key, 1, 100);
    char newline = '\n';
    HAL_UART_Transmit(&huart1, (uint8_t*)&newline, 1, 100);
}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart1,tx_data,sizeof(tx_data),100);
  HAL_UART_Receive_IT(&huart1,&rx_data,1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  key = scan_keypad();
	  if(key != '\0'){
//		  if(key != last_key){
			  display_key(key);
			  key = '\0';
//		  }
	  }
//	  last_key = key;
	  if(data_received_flag){
		  HAL_UART_Transmit(&huart1, &rx_data, sizeof(rx_data), 100);
		  data_received_flag=0;
	  }
	  HAL_Delay(100);
//	  key = '\0';
  }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
