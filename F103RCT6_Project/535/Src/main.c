/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define LED1_ON() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)
#define LED1_OFF() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)
#define LED2_ON() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)
#define LED2_OFF() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)

uint8_t Tx_str1[] = "hello chen!\r\n";
uint8_t Tx_str2[] = "LED1 Open!\r\n";
uint8_t Tx_str3[] = "LED1 Closed!\r\n";
uint8_t Rx_dat = 0;
uint8_t Str_buff1[64];

uint16_t voltage = 0; //ADC转换值
uint16_t ADC_Collect = 0; //ADC采集值

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        if(Rx_dat == 0xa1)
        {
            LED1_ON();
			
            LED2_ON();
	    HAL_UART_Transmit(&huart1,Tx_str2,sizeof(Tx_str2),10000);
	   // LED2_OFF();
	    HAL_UART_Receive_IT(&huart1,&Rx_dat,1);
        }
        else if(Rx_dat == 0xa2)
        {
	    LED1_OFF();
			
	    LED2_ON();
	    HAL_UART_Transmit(&huart1,Tx_str3,sizeof(Tx_str3),10000);
	    LED2_OFF();
	    HAL_UART_Receive_IT(&huart1,&Rx_dat,1);
        }
    }
}
uint16_t Get_Voltage(void) {

    //鍚姩ADC
    HAL_ADC_Start(&hadc1);
    //绛夊緟ADC閲囬泦瀹屾垚
    HAL_ADC_PollForConversion(&hadc1, 10);
    //鑾峰彇ADC閲囬泦鏁版嵁
    ADC_Collect = HAL_ADC_GetValue(&hadc1);
    //灏嗛噰闆嗗埌鐨勬暟鎹浆鎹负鐢靛帇鍊?
    voltage = (ADC_Collect * 330) / 4096;
    //鍋滄ADC
    HAL_ADC_Stop(&hadc1);
    return voltage;
}

void Get_Light() {
    LED1_ON();
    sprintf((char *) Str_buff1, "采样值：%d，光照值：%d\r\n", ADC_Collect, voltage);
    HAL_UART_Transmit(&huart1, Str_buff1, sizeof(Str_buff1), 0xFFFF);
    LED1_OFF();
    HAL_Delay(200);
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
//閸︹暕ian()閸戣姤鏆熸稉顓熷潑閸旂姳浜掓稉瀣╁敩閻緤锟??
LED2_ON();
HAL_Delay(500);
HAL_UART_Transmit(&huart1,Tx_str1,sizeof(Tx_str1),10000);    //閸氭垳绗傛担宥嗘簚閸欐垿?锟???娓塭llo world!锟???
LED2_OFF();
HAL_UART_Receive_IT(&huart1,&Rx_dat,1);                      //閸氼垰濮╂稉鎻掑經1閹恒儲鏁规稉濠佺秴锟???1娑擃亜鐡ч懞?
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//      HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
//      HAL_Delay(500);
      /*Get_Light();
      HAL_Delay(500);*/
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
