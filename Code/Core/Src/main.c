/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "DEV_Config.h"
#include "LCD_Driver.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
float temp = 0.0;
int16_t lamp = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Delay_us (int16_t delay) 
{
  htim14.Instance->CNT = 0;
	while(htim14.Instance->CNT <= delay);
}
void ONE_WIRE_SET(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
}

void ONE_WIRE_RESET(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
}

uint8_t DS18B20_Init(void)
{
	ONE_WIRE_OUTPUT();
	ONE_WIRE_SET();
	Delay_us(5);
	ONE_WIRE_RESET();
	Delay_us(750);
	ONE_WIRE_SET();
	ONE_WIRE_INPUT();
	
	int i = 0;
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8))
	{
		i++;
		if(i > 60)
			return 0;
		Delay_us(1);
	}
	i = 480 - i;
	ONE_WIRE_OUTPUT();
	Delay_us(i);
	ONE_WIRE_SET();
	return 1;
}

void DS18B20_Write(uint8_t data)
{
  ONE_WIRE_OUTPUT();
  for (int i = 0; i < 8; i++)
  {
    ONE_WIRE_RESET();
    Delay_us(10);
    if (data & 1) ONE_WIRE_SET();
    else ONE_WIRE_RESET();
    data >>= 1;
    Delay_us(50);
    ONE_WIRE_SET();
  }
}

uint8_t DS18B20_Read()
{
  ONE_WIRE_OUTPUT();
  ONE_WIRE_SET();
  Delay_us(2);
  uint8_t data = 0;
  for (int i = 0; i < 8; i++)
  {
    ONE_WIRE_RESET();
    Delay_us(1);
    ONE_WIRE_SET();
    ONE_WIRE_INPUT();
    Delay_us(5);
    data >>= 1;
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8)) data |= 0x80;
    Delay_us(55);
    ONE_WIRE_OUTPUT();
    ONE_WIRE_SET();
  }
  return data;
}
 
int TempRead()
{
  if (!DS18B20_Init()) return 0;
  DS18B20_Write (0xCC); // Send skip ROM command
  DS18B20_Write (0x44); // Send reading start conversion command
  if (!DS18B20_Init()) return 0;
  DS18B20_Write (0xCC); // Send skip ROM command
  DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
  int temp = DS18B20_Read (); // Low byte
  temp |= DS18B20_Read () << 8; // High byte
  return temp;
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
  MX_USART2_UART_Init();
  MX_TIM14_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM13_Init();
	///PWM
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_Base_Start_IT(&htim13);
  /* USER CODE END 2 */
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	
	LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;//SCAN_DIR_DFT = D2U_L2R	
	LCD_Init(Lcd_ScanDir, 1000);
	TP_Init(Lcd_ScanDir);
	
	TP_GetAdFac();
	TP_Show_Main();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		DEV_TIME sDev_time;
//		sDev_time.Hour = 23;
//		sDev_time.Min = 38;
//		sDev_time.Sec = 56;

//		for (;;) 
//		{
//			sDev_time.Sec++;
//			GUI_Showtime(200, 150, 327, 197, &sDev_time, RED);
//			Driver_Delay_ms(1000);//Analog clock 1s
//			if (sDev_time.Sec == 60)
//				sDev_time.Sec = 0;
//		}
		//for (;;)
			TP_Temp();
		
		//lamp += 100;
//		for (int i = 0 ; i < 100 ; i++)
//		{
//			__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, i * 10);
//			HAL_Delay(100);
//		}
//		
//		if (lamp == 1000)
//			lamp = 0;
		
		//HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM13)
	{
		temp = TempRead() * 0.0625;
		TP_Show_Temp(temp);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
