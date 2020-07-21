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
    int axis_type;


public:
    Axis(TIM_HandleTypeDef *timer, struct ControlPin pins[], int axis_type);
    ~Axis();

    static const int AXIS_TYPE_RA   = 0;
    static const int AXIS_TYPE_DEC  = 1;

    int GoTo(int arcsec);
    int GoTo(int degree, int minutes, int sec);
    int GoTo(double degree);
    int Start();
    int Stop();
    int TimerInterrupt();
};

