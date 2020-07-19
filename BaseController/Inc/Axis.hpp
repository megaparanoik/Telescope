#pragma once

#include "tim.h"
#include "IMotorDriver.hpp"
#include "drv8825.hpp"

class Axis
{
private:
    static long int arcsec_max;
    TIM_HandleTypeDef *axis_timer;
    IMotorDriver *axis_motor;

    int current_arcsec;
    int target_arcsec;
    int delta;
    int remains_steps;


public:
    Axis(TIM_HandleTypeDef *timer, struct MotorDriverPins *pins);
    ~Axis();

    int GoTo(int arcsec);
    int Start();
    int Stop();
    int TimerInterrupt();
};

