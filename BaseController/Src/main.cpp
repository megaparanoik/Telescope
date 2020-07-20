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

void SystemClock_Config(void);

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

int main(void)
{
	
  	HAL_Init();
  	SystemClock_Config();
  	MX_GPIO_Init();
  	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_DMA_Init();
	MX_USART3_UART_Init();

	Logger &logger = Logger::GetInstance();
	logger.Init(&huart3);


	logger.Write("Telescope App");

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

	while(1) {	
		DEC_axis->GoTo_arcsec(10000);
		LL_mDelay(5000);

		DEC_axis->GoTo_arcsec(20000);
		LL_mDelay(5000);

		//DEC_axis->GoTo_arcsec(50000);
		//LL_mDelay(5000);	
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
