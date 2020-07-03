#include "main.h"
#include "gpio.h"
#include "drv8825.hpp"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"

drv8825 *DEC_motor;

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  DEC_motor = new drv8825();
  DEC_motor->SetupResetPin(GPIOB, GPIO_PIN_15);
  DEC_motor->SetupSleepPin(GPIOB, GPIO_PIN_14);
  DEC_motor->SetupEnablePin(GPIOA, GPIO_PIN_11);
  DEC_motor->SetupDirPin(GPIOB, GPIO_PIN_12);
  DEC_motor->SetupStepPin(GPIOB, GPIO_PIN_13);

  DEC_motor->SetupM0Pin(GPIOA, GPIO_PIN_10);
  DEC_motor->SetupM1Pin(GPIOA, GPIO_PIN_9);
  DEC_motor->SetupM2Pin(GPIOA, GPIO_PIN_8);

  DEC_motor->Reset();
  DEC_motor->SetSleepMode(drv8825::SLEEP_MODE_WAKE);
  DEC_motor->SetMicrostep(drv8825::STEP_1_32);
  DEC_motor->SetDirection(drv8825::DIRECTION_CLOCKWISE);
  DEC_motor->Enable();
  
  
  while(1) {
    DEC_motor->doStep();
    LL_mDelay(1);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
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
