#include "Axis.hpp"


Axis::Axis(TIM_HandleTypeDef *timer, struct MotorDriverPins *pins)
{
    axis_timer = timer;
    axis_motor = new drv8825(pins);

    Stop();

    axis_motor->Reset();
    axis_motor->SetMicrostep(IMotorDriver::STEP_1_32);
    axis_motor->SetDirection(IMotorDriver::DIRECTION_COUNTERCLOCKWISE);
    axis_motor->SetSleepMode(IMotorDriver::SLEEP_MODE_WAKE);
    axis_motor->Enable();

    current_arcsec  = 0;
    delta           = 0;
    remains_steps   = 0;
    target_arcsec   = 0;
}

Axis::~Axis()
{
}

int Axis::GoTo(int arcsec)
{
    target_arcsec = arcsec;
    delta = current_arcsec - target_arcsec;
    if (delta > 0) {
        axis_motor->SetDirection(IMotorDriver::DIRECTION_COUNTERCLOCKWISE);
    } else if (delta < 0) {
        axis_motor->SetDirection(IMotorDriver::DIRECTION_CLOCKWISE);
    } else {
        return 0;
    }

    remains_steps = delta / axis_motor->GetResolution();
    if (remains_steps < 0) {
        remains_steps *= -1;
    }
    remains_steps *= 2;

    if (remains_steps != 0) {
        //Set_timer_speed(axis_timer, TIMER_TRACKING_SPEED);
        Set_timer_speed(axis_timer, TIMER_MOVE_SPEED);
        Start();
    }

    return 0;
}

int Axis::Start()
{
    HAL_TIM_Base_Start_IT(axis_timer);
    return 0;
}

int Axis::Stop()
{
    HAL_TIM_Base_Stop_IT(axis_timer);
    return 0;
}

int Axis::TimerInterrupt()
{
    if (remains_steps > 0) {
        remains_steps--;
        axis_motor->ToggleStepPin();
    } else {
        current_arcsec = target_arcsec;
        Stop();
    }
}