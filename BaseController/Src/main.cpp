#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "drv8825.hpp"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "Axis.hpp"

static int STARRY_DAY           = 86164;	//86164,090530833 sec
static int MOTOR_STEPS_PER_REV  = 200;    	//360/1.8deg = 200 steps
static int GEAR_RATIO           = 2;      	//ratio 2:1
static int RA_RATIO 			= 130;    	//
static int DEC_RATIO			= 65;		//
static int MOTOR_STEPS			= 200;		//



extern TIM_HandleTypeDef htim2;

drv8825 *DEC_motor;
drv8825 *RA_motor;
Axis *RA_axis;

void SystemClock_Config(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2) {
		RA_motor->ToggleStepPin();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);                
  	}
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  RA_motor = new drv8825();
  RA_motor->SetupResetPin(GPIOB, GPIO_PIN_15);
  RA_motor->SetupSleepPin(GPIOB, GPIO_PIN_14);
  RA_motor->SetupEnablePin(GPIOA, GPIO_PIN_11);
  RA_motor->SetupDirPin(GPIOB, GPIO_PIN_12);
  RA_motor->SetupStepPin(GPIOB, GPIO_PIN_13);

  RA_motor->SetupM0Pin(GPIOA, GPIO_PIN_10);
  RA_motor->SetupM1Pin(GPIOA, GPIO_PIN_9);
  RA_motor->SetupM2Pin(GPIOA, GPIO_PIN_8);

  RA_motor->Reset();
  RA_motor->SetSleepMode(drv8825::SLEEP_MODE_WAKE);
  RA_motor->SetMicrostep(drv8825::STEP_1_32);
  //RA_motor->SetDirection(drv8825::DIRECTION_COUNTERCLOCKWISE);
  RA_motor->SetDirection(drv8825::DIRECTION_CLOCKWISE);
  
  RA_motor->Enable();

  HAL_TIM_Base_Start_IT(&htim2);

  RA_axis = new Axis(RA_motor);

  //Set_RA_timer_speed(RA_TIMER_MODE_MOVE);
  //Set_RA_timer_speed(RA_TIMER_MODE_TRACKING);

  while(1) {
	  RA_motor->Disable();
	  RA_motor->SetDirection(drv8825::DIRECTION_CLOCKWISE);
	  LL_mDelay(1);
	  RA_motor->Enable();
	  Set_RA_timer_speed(RA_TIMER_MOVE_SPEED);
	  LL_mDelay(5000);
	  Set_RA_timer_speed(RA_TIMER_TRACKING_SPEED);
	  LL_mDelay(5000);


	  RA_motor->Disable();
	  RA_motor->SetDirection(drv8825::DIRECTION_COUNTERCLOCKWISE);
	  LL_mDelay(1);
	  RA_motor->Enable();
	  Set_RA_timer_speed(RA_TIMER_MOVE_SPEED);
	  LL_mDelay(5000);
	  Set_RA_timer_speed(RA_TIMER_TRACKING_SPEED);
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
