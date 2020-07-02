#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define M_Dir_Pin GPIO_PIN_12
#define M_Dir_GPIO_Port GPIOB
#define M_Step_Pin GPIO_PIN_13
#define M_Step_GPIO_Port GPIOB
#define M_Sleep_Pin GPIO_PIN_14
#define M_Sleep_GPIO_Port GPIOB
#define M_Reset_Pin GPIO_PIN_15
#define M_Reset_GPIO_Port GPIOB
#define M2_Pin GPIO_PIN_8
#define M2_GPIO_Port GPIOA
#define M1_Pin GPIO_PIN_9
#define M1_GPIO_Port GPIOA
#define M0_Pin GPIO_PIN_10
#define M0_GPIO_Port GPIOA
#define M_Enable_Pin GPIO_PIN_11
#define M_Enable_GPIO_Port GPIOA

#ifdef __cplusplus
}
#endif
