#pragma once

#include "tim.h"
#include "IMotorDriver.hpp"
#include "drv8825.hpp"

class Axis
{
private:
    const unsigned int RA_AXIS_RATIO = 130;
    const unsigned int DEC_AXIS_RATIO = 65;
    const unsigned int GEARBOX_RATIO = 2;
    const unsigned int ARCSE_PER_AXIS_REV = 1296000;
    const unsigned int DEGRE_PER_AXIS_REV = 360;
    const unsigned int MOTOR_FULL_STEPS = 200;
    double resolution; //in degree/step
    TIM_HandleTypeDef *axis_timer;
    IMotorDriver *axis_motor;

    double current_degree;
    double target_degree;
    double delta;
    int remains_steps;
    int axis_type;
    int movement_direction;


public:
    Axis(TIM_HandleTypeDef *timer, struct ControlPin pins[], int axis_type);
    ~Axis();

    static const int AXIS_TYPE_RA   = 0;
    static const int AXIS_TYPE_DEC  = 1;

    int GoTo(double degree);
    int Start();
    int Stop();
    int TimerInterrupt();
    double GetCurrentPosition();
    int SetCurentPosition(double degree);
};

