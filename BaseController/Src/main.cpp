#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "Axis.hpp"
#include "IMotorDriver.hpp"

static int STARRY_DAY           = 86164;	  //86164,090530833 sec
static int MOTOR_STEPS_PER_REV  = 200;    	//360/1.8deg = 200 steps
static int GEAR_RATIO           = 2;      	//ratio 2:1
static int RA_RATIO 			= 130;    	//
static int DEC_RATIO			= 65;		    //
static int MOTOR_STEPS			= 200;		  //



extern TIM_HandleTypeDef htim2;
struct MotorDriverPins RA_Motor_pins;
Axis *RA_axis;


void SystemClock_Config(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

	if(htim->Instance == TIM2) {
    	if (RA_axis) {
      		RA_axis->TimerInterrupt();
    	}	
  	}
}

int main(void)
{
  	HAL_Init();
  	SystemClock_Config();
  	MX_GPIO_Init();
  	MX_TIM2_Init();

  	RA_Motor_pins.Reset_PORT  = GPIOB;
  	RA_Motor_pins.Reset_PIN   = GPIO_PIN_15;
  	RA_Motor_pins.Sleep_PORT  = GPIOB;
  	RA_Motor_pins.Sleep_PIN   = GPIO_PIN_14;
  	RA_Motor_pins.Step_PORT   = GPIOB;
  	RA_Motor_pins.Step_PIN    = GPIO_PIN_13;
	RA_Motor_pins.Dir_PORT    = GPIOB;
  	RA_Motor_pins.Dir_PIN     = GPIO_PIN_12;

	RA_Motor_pins.Enable_PORT = GPIOA;
  	RA_Motor_pins.Enable_PIN  = GPIO_PIN_11;
  	RA_Motor_pins.M0_PORT     = GPIOA;
  	RA_Motor_pins.M0_PIN      = GPIO_PIN_10;
  	RA_Motor_pins.M1_PORT     = GPIOA;
  	RA_Motor_pins.M1_PIN      = GPIO_PIN_9;
  	RA_Motor_pins.M2_PORT     = GPIOA;
  	RA_Motor_pins.M2_PIN      = GPIO_PIN_8;

	RA_axis = new Axis(&htim2, &RA_Motor_pins);

	while(1) {
		RA_axis->GoTo(10000);
		LL_mDelay(5000);

		RA_axis->GoTo(20000);
		LL_mDelay(5000);

		RA_axis->GoTo(50000);
		LL_mDelay(5000);	
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
