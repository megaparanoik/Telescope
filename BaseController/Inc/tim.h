#pragma once 

static int RA_TIMER_TRACKING_SPEED  =   25890;
static int RA_TIMER_MOVE_SPEED      =   50;
static int RA_TIMER_MODE_TRACKING   =   0;
static int RA_TIMER_MODE_MOVE       =   1;

#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
extern TIM_HandleTypeDef htim2;

void MX_TIM2_Init(void);

void Set_RA_timer_speed(int speed);
#ifdef __cplusplus
}
#endif
