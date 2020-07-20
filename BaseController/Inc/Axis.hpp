#pragma once

#include "tim.h"
#include "IMotorDriver.hpp"
#include "drv8825.hpp"

class Axis
{
private:
    static const int DECLINATION_MIN  = 0;  //degrees
    static const int DECLINATION_MAX  = 90; //degrees

    static int arcsec_max;
    TIM_HandleTypeDef *axis_timer;
    IMotorDriver *axis_motor;

    int current_arcsec;
    int target_arcsec;
    int delta;
    int remains_steps;


public:
    Axis(TIM_HandleTypeDef *timer, struct ControlPin pins[]);
    ~Axis();

    int GoTo_arcsec(int arcsec);
    int GoTo_degree(double degree);
    int Start();
    int Stop();
    int TimerInterrupt();
};

