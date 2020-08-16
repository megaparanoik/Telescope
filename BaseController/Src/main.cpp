#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "dma.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "Axis.hpp"
#include "IMotorDriver.hpp"
#include "Logger.hpp"
#include "RingBuff.hpp"
#include <cstdio>

static int STARRY_DAY           = 86164;	//86164,090530833 sec
static int MOTOR_STEPS_PER_REV  = 200;    	//360/1.8deg = 200 steps
static int GEAR_RATIO           = 2;      	//ratio 2:1
static int RA_RATIO 			= 130;    	//
static int DEC_RATIO			= 65;		//
static int MOTOR_STEPS			= 200;		//


extern UART_HandleTypeDef huart3;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
Axis *RA_axis;
Axis *DEC_axis;

struct ControlPin RA_Motor_pins[MAX_control_pin] = {0};
struct ControlPin DEC_Motor_pins[MAX_control_pin] = {0};

RingBuff<uint8_t> *ring;
uint8_t buff[2] = {0};

void Parse(uint8_t *command);
void SystemClock_Config(void);
void Send_RA_DEC();
void Set_RA_DEC(unsigned int RA, unsigned int DEC);
void Send_Confirmation();

unsigned int RA = 0x34AB0500;
unsigned int DEC = 0x12CE0500;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	

	if(htim->Instance == TIM2) {
    	if (RA_axis) {
      		RA_axis->TimerInterrupt();
    	}	
  	} else if(htim->Instance == TIM3) {
    	if (DEC_axis) {
      		DEC_axis->TimerInterrupt();
    	}	
  	} 
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3) {
    }
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3) {
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart3) {
		if (ring) {
			ring->Push(buff[0]);
			HAL_UART_Receive_IT(&huart3, (uint8_t*)buff, 1);
		}
    }
}

int main(void)
{
  	HAL_Init();
  	SystemClock_Config();
  	MX_GPIO_Init();
  	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_DMA_Init();
	MX_USART3_UART_Init();

	RA_Motor_pins[RESET_control_pin].PORT 	= GPIOB;
	RA_Motor_pins[RESET_control_pin].PIN 	= GPIO_PIN_15;
	RA_Motor_pins[SLEEP_control_pin].PORT 	= GPIOB;
	RA_Motor_pins[SLEEP_control_pin].PIN 	= GPIO_PIN_14;
	RA_Motor_pins[STEP_control_pin].PORT 	= GPIOB;
	RA_Motor_pins[STEP_control_pin].PIN 	= GPIO_PIN_13;
	RA_Motor_pins[DIR_control_pin].PORT 	= GPIOB;
	RA_Motor_pins[DIR_control_pin].PIN 		= GPIO_PIN_12;
	RA_Motor_pins[ENABLE_control_pin].PORT 	= GPIOA;
	RA_Motor_pins[ENABLE_control_pin].PIN 	= GPIO_PIN_11;
	RA_Motor_pins[M0_control_pin].PORT 		= GPIOA;
	RA_Motor_pins[M0_control_pin].PIN 		= GPIO_PIN_10;
	RA_Motor_pins[M1_control_pin].PORT 		= GPIOA;
	RA_Motor_pins[M1_control_pin].PIN 		= GPIO_PIN_9;
	RA_Motor_pins[M2_control_pin].PORT 		= GPIOA;
	RA_Motor_pins[M2_control_pin].PIN 		= GPIO_PIN_8;

	DEC_Motor_pins[RESET_control_pin].PORT 	= GPIOB;
	DEC_Motor_pins[RESET_control_pin].PIN 	= GPIO_PIN_4;
	DEC_Motor_pins[SLEEP_control_pin].PORT 	= GPIOB;
	DEC_Motor_pins[SLEEP_control_pin].PIN 	= GPIO_PIN_3;
	DEC_Motor_pins[STEP_control_pin].PORT 	= GPIOA;
	DEC_Motor_pins[STEP_control_pin].PIN 	= GPIO_PIN_15;
	DEC_Motor_pins[DIR_control_pin].PORT 	= GPIOA;
	DEC_Motor_pins[DIR_control_pin].PIN		= GPIO_PIN_12;
	DEC_Motor_pins[ENABLE_control_pin].PORT	= GPIOB;
	DEC_Motor_pins[ENABLE_control_pin].PIN 	= GPIO_PIN_9;
	DEC_Motor_pins[M0_control_pin].PORT 	= GPIOB;
	DEC_Motor_pins[M0_control_pin].PIN 		= GPIO_PIN_8;
	DEC_Motor_pins[M1_control_pin].PORT 	= GPIOB;
	DEC_Motor_pins[M1_control_pin].PIN 		= GPIO_PIN_7;
	DEC_Motor_pins[M2_control_pin].PORT 	= GPIOB;
	DEC_Motor_pins[M2_control_pin].PIN 		= GPIO_PIN_6;

	RA_axis  = new Axis(&htim2, RA_Motor_pins, Axis::AXIS_TYPE_RA);
	DEC_axis = new Axis(&htim3, DEC_Motor_pins, Axis::AXIS_TYPE_DEC);

	ring  = new RingBuff<uint8_t>(100);
	HAL_UART_Receive_IT(&huart3, (uint8_t*)buff, 1);


	RA_axis->SetCurentPosition(50);
	DEC_axis->SetCurentPosition(10);

	while (1)
	{
		LL_mDelay(1000);

		uint8_t buff = 0;
		if(ring->Peek(&buff) != 0) {
			Parse(&buff);
		}
	}

	
	while(1) {	
  	}
}

void Send_Confirmation()
{
	char buffer[100] = {0};
	snprintf(buffer, 100, "#");
	HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), 1000);
}

void Parse(uint8_t *command)
{
	uint8_t buffer[100] = {0};
	unsigned int RA = 0;
	unsigned int DEC = 0;

	switch (*command)
	{
	//Get precise RA/DEC
	case 'e':
		ring->Pop(buffer, 1);

		RA = (RA_axis->GetCurrentPosition() / 360) * 4294967296;
		DEC = (DEC_axis->GetCurrentPosition() / 360) * 4294967296;
		snprintf((char *)buffer, 100, "%08X,%08X#", RA, DEC);
		HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen((char *)buffer), 1000);
		break;

	//Sync precise RA/DEC
	case 's':
		ring->Pop(buffer, 18);
		
		sscanf((char*)buffer, "s%8x,%8x", &RA, &DEC);
		RA_axis->SetCurentPosition(((double)RA/4294967296)*360);
		DEC_axis->SetCurentPosition(((double)DEC/4294967296)*360);
		Send_Confirmation();
		break;

	//Goto precise RA/DEC
	case 'r':
		ring->Pop(buffer, 18);
		sscanf((char*)buffer, "r%8x,%8x", &RA, &DEC);
		RA_axis->GoTo(((double)RA/4294967296)*360);
		DEC_axis->GoTo(((double)DEC/4294967296)*360);
		Send_Confirmation();
		break;
	
	default:
		LL_mDelay(10);
		break;
	}
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
}

void Error_Handler(void)
{
}
