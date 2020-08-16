#include "Axis.hpp"
#include "Logger.hpp"
#include "math.h"

Axis::Axis(TIM_HandleTypeDef *timer, struct ControlPin pins[], int axis_type)
{
    this->axis_type = axis_type;
    axis_timer = timer;
    axis_motor = new drv8825(pins);

    Stop();

    axis_motor->Reset();
    axis_motor->SetMicrostep(IMotorDriver::STEP_1_32);
    axis_motor->SetDirection(IMotorDriver::DIRECTION_COUNTERCLOCKWISE);
    axis_motor->SetSleepMode(IMotorDriver::SLEEP_MODE_WAKE);

    current_degree  = 45;
    target_degree   = 0;
    toggle_var = 0;

    unsigned int motor_rev_per_axis_rev;
    if (axis_type == AXIS_TYPE_RA) {
        motor_rev_per_axis_rev = RA_AXIS_RATIO * GEARBOX_RATIO;
    } else if (axis_type == AXIS_TYPE_DEC) {
        motor_rev_per_axis_rev = DEC_AXIS_RATIO * GEARBOX_RATIO;
    }
    resolution = (double)DEGRE_PER_AXIS_REV/(motor_rev_per_axis_rev * (MOTOR_FULL_STEPS * 32));
}

Axis::~Axis()
{
}

int Axis::GoTo(double degree)
{
    Stop();
    target_degree = degree;

    if (fabs(target_degree - current_degree) < resolution) {
        return 0;
    }
    
    //Set direction
    if ((target_degree - current_degree) > 0) {
        movement_direction = IMotorDriver::DIRECTION_COUNTERCLOCKWISE;
        axis_motor->SetDirection(IMotorDriver::DIRECTION_COUNTERCLOCKWISE);
    } else {
        movement_direction = IMotorDriver::DIRECTION_CLOCKWISE;
        axis_motor->SetDirection(IMotorDriver::DIRECTION_CLOCKWISE);
    }

    Set_timer_speed(axis_timer, TIMER_MOVE_SPEED);
    Start();

    return 0;
}

int Axis::Start()
{
    axis_motor->Enable();
    HAL_TIM_Base_Start_IT(axis_timer);
    return 0;
}

int Axis::Stop()
{
    axis_motor->Disable();
    HAL_TIM_Base_Stop_IT(axis_timer);
    return 0;
}

int Axis::TimerInterrupt()
{
    toggle_var = !toggle_var;

    if (toggle_var) {
        if (movement_direction == IMotorDriver::DIRECTION_COUNTERCLOCKWISE) {
            current_degree += resolution;
        } else {
            current_degree -= resolution;
        }
    }

    if (fabs(current_degree - target_degree)  < resolution) {
        toggle_var = 0;
        Stop();
    }

    axis_motor->ToggleStepPin();

    return 0;
}

double Axis::GetCurrentPosition()
{
    return current_degree;
}

int Axis::SetCurentPosition(double degree)
{
    current_degree = degree;
    return 0;
}