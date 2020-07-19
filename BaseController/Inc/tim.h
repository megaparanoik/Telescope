#pragma once 

static int TIMER_TRACKING_SPEED  =   25890;
static int TIMER_MOVE_SPEED      =   50;

#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
extern TIM_HandleTypeDef htim2;

void MX_TIM2_Init(void);

void Set_timer_speed(TIM_HandleTypeDef *timer, int speed);
#ifdef __cplusplus
}
#endif
