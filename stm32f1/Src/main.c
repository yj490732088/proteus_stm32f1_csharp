/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "string.h"
#include <stdlib.h>
#define EN_Pin GPIO_PIN_6
#define EN_GPIO_Port GPIOA
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t Second;
uint8_t Minute;
uint8_t Hour;
uint8_t Date;
uint8_t Day;
uint8_t Month;
uint8_t Year;	
RTC_TimeTypeDef sTime;
RTC_DateTypeDef DateToUpdate;
char cTime[32];
uint8_t Rx_data[12];
uint8_t send = 'A';
char uart_rx_buffer [60];
uint8_t Rx_indx;
uint8_t cmd;
uint32_t add;
uint8_t value;
uint32_t on_hour[3],on_minute[3],on_second[3];
uint32_t off_hour[3],off_minute[3],off_second[3];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_RTC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void task(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void LCD_Enable()
{
HAL_GPIO_WritePin(GPIOA,EN_Pin,1);
HAL_Delay(1);
HAL_GPIO_WritePin(GPIOA,EN_Pin,0);	
HAL_Delay(1);	
}

void LCD_Send4Bit(unsigned char Data)
{
HAL_GPIO_WritePin(GPIOB,D4_Pin,Data&0x01);
HAL_GPIO_WritePin(GPIOB,D5_Pin,(Data>>1)&0x01);
HAL_GPIO_WritePin(GPIOB,D6_Pin,(Data>>2)&0x01);
HAL_GPIO_WritePin(GPIOB,D7_Pin,(Data>>3)&0x01);	
}

void LCD_SendCommand(unsigned char command)
{
	LCD_Send4Bit(command >>4);/* Gui 4 bit cao */
	LCD_Enable();
	LCD_Send4Bit(command);	/* Gui 4 bit thap*/
	LCD_Enable();
}
void LCD_Clear()
{
 	LCD_SendCommand(0x01);  
  HAL_Delay(1);	
}

void LCD_Init()
{
	LCD_Send4Bit(0x00);
  HAL_GPIO_WritePin(GPIOA,RS_Pin,0);
	LCD_Send4Bit(0x03);
	LCD_Enable();
	LCD_Enable();
	LCD_Enable();
	LCD_Send4Bit(0x02);
	LCD_Enable();
	LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
	LCD_SendCommand(0x0C); // cho phep hien thi man hinh
	LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
	LCD_SendCommand(0x01); // xoa toan bo khung hinh
}

void LCD_Gotoxy(unsigned char x, unsigned char y)
{
	unsigned char address;
  	if(!y)address=(0x80+x);
  	else address=(0xC0+x);
  	LCD_SendCommand(address);

}

void LCD_PutChar(unsigned char Data)
{
  HAL_GPIO_WritePin(GPIOA,RS_Pin,1);
 	LCD_SendCommand(Data);
  HAL_GPIO_WritePin(GPIOA,RS_Pin,0);
}

void LCD_Puts(char *s)
{
   	while (*s){
      	LCD_PutChar(*s);
     	s++;
   	}
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_RTC_Init();

  /* USER CODE BEGIN 2 */
	LCD_Init();
	HAL_UART_Receive_IT (&huart1,Rx_data,5);
	//on_hour[0]=1;
	//on_hour[1]=1;
	//on_hour[2]=1;
	//on_second[0] = 2;
	//on_second[1] = 5;
	//on_second[2] = 10;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		//HAL_UART_Receive_IT (&huart1, Rx_data, 6);
		memset(cTime,0,sizeof(cTime));
		HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);
		Second=sTime.Seconds;
		Minute=sTime.Minutes;
		Hour=sTime.Hours;
		Day=DateToUpdate.Date;
		Date=DateToUpdate.WeekDay;
		Month=DateToUpdate.Month;
		Year=DateToUpdate.Year;
		sprintf(cTime,"Time  %02d:%02d:%02d",Hour,Minute,Second);
		LCD_Gotoxy(0,0);
		LCD_Puts(cTime);
		memset(cTime,0,sizeof(cTime));
		sprintf(cTime,"Date %02d/%02d/20%02d",Day,Month,Year);
		LCD_Gotoxy(0,1);
		LCD_Puts(cTime);
		task();
		uint32_t t,t_on,t_off;
		t = Hour*60*60 + Minute*60 + Second;
		for (int i = 0;i<3;i++){
			if ((on_hour[i] != 0) || (on_minute[i] != 0) ||(on_second[i] != 0)){
				t_on = on_hour[i]*60*60 + on_minute[i]*60 + on_second[i];
				if (t >= t_on && t < (t_on+2)){
					switch(i){
						case 0:{
							HAL_GPIO_WritePin(GPIOA,R1_Pin,GPIO_PIN_SET);break;
						}
						case 1:{
							HAL_GPIO_WritePin(GPIOA,R2_Pin,GPIO_PIN_SET);break;
						}
						case 2:{
							HAL_GPIO_WritePin(GPIOA,R3_Pin,GPIO_PIN_SET);break;
						}
					}
				}
			}
			if ((off_hour[i] != 0) || (off_minute[i] != 0) ||(off_second[i] != 0)){
				t_off = off_hour[i]*60*60 + off_minute[i]*60 + off_second[i];
				if (t >= t_off && t < (t_off+2)){
					switch(i){
						case 0:{
							HAL_GPIO_WritePin(GPIOA,R1_Pin,GPIO_PIN_RESET);break;
						}
						case 1:{
							HAL_GPIO_WritePin(GPIOA,R2_Pin,GPIO_PIN_RESET);break;
						}
						case 2:{
							HAL_GPIO_WritePin(GPIOA,R3_Pin,GPIO_PIN_RESET);break;
						}
					}
				}
			}
		}
		HAL_Delay(300);
		//HAL_UART_Transmit(&huart1,&send,1,500);
		//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* RTC init function */
static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2){
  sTime.Hours = 0x01;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_FEBRUARY;
  DateToUpdate.Date = 0x28;
  DateToUpdate.Year = 0x20;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0x32F2);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

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
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, R1_Pin|R2_Pin|R3_Pin|RS_Pin 
                          |E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D4_Pin|D5_Pin|D6_Pin|D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin RS_Pin 
                           E_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|RS_Pin 
                          |E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D4_Pin D5_Pin D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = D4_Pin|D5_Pin|D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Transmit(&huart1,&send,1,500);
	//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);
	if (huart->Instance == USART1)             
	{
			cmd = Rx_data[0];
			value = Rx_data[4];
			add = (uint32_t)(Rx_data[1])<<16 | (uint32_t)(Rx_data[2])<<8 | (uint32_t)(Rx_data[3]);
			HAL_UART_Transmit(&huart1,Rx_data,5,500);
			memset(Rx_data,0,sizeof(Rx_data));
			HAL_UART_Receive_IT (&huart1, Rx_data,5);
	}
	
}
void task(void){
	if (cmd != 0x00){
		if (cmd == 0xF1)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				HAL_GPIO_WritePin(GPIOA,R1_Pin,value);
			}
			if (add == 0x00160602){
				HAL_GPIO_WritePin(GPIOA,R2_Pin,value);
			}
			if (add == 0x00160603){
				HAL_GPIO_WritePin(GPIOA,R3_Pin,value);
			}
		}
		if (cmd == 0xE1)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				on_hour[0] = value;
			}
			if (add == 0x00160602){
				on_hour[1] = value;
			}
			if (add == 0x00160603){
				on_hour[2] = value;
			}
		}
		if (cmd == 0xE2)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				on_minute[0] = value;
			}
			if (add == 0x00160602){
				on_minute[1] = value;
			}
			if (add == 0x00160603){
				on_minute[2] = value;
			}
		}
		if (cmd == 0xE3)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				on_second[0] = value;
			}
			if (add == 0x00160602){
				on_second[1] = value;
			}
			if (add == 0x00160603){
				on_second[2] = value;
			}
		}
		if (cmd == 0xD1)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				off_hour[0] = value;
			}
			if (add == 0x00160602){
				off_hour[1] = value;
			}
			if (add == 0x00160603){
				off_hour[2] = value;
			}
		}
		if (cmd == 0xD2)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				off_minute[0] = value;
			}
			if (add == 0x00160602){
				off_minute[1] = value;
			}
			if (add == 0x00160603){
				off_minute[2] = value;
			}
		}
		if (cmd == 0xD3)
		{
			cmd = 0x00;
			if (add == 0x00160601){
				off_second[0] = value;
			}
			if (add == 0x00160602){
				off_second[1] = value;
			}
			if (add == 0x00160603){
				off_second[2] = value;
			}
		}
	}
}
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
