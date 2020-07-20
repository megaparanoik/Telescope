#include "Axis.hpp"
#include "Logger.hpp"

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

    current_arcsec  = 0;
    delta           = 0;
    remains_steps   = 0;
    target_arcsec   = 0;
}

Axis::~Axis()
{
}

int Axis::GoTo_arcsec(int arcsec)
{
    Logger &logger = Logger::GetInstance();

    logger.write("Goto %d arces", arcsec);

    target_arcsec = arcsec;
    delta = current_arcsec - target_arcsec;

    logger.write("Delta: %d", delta);

    if (delta > 0) {
        axis_motor->SetDirection(IMotorDriver::DIRECTION_COUNTERCLOCKWISE);
    } else if (delta < 0) {
        axis_motor->SetDirection(IMotorDriver::DIRECTION_CLOCKWISE);
    } else {
        return 0;
    }

    remains_steps = delta / axis_motor->GetResolution();

    logger.write("Remains steps: %d", remains_steps);

    if (remains_steps < 0) {
        remains_steps *= -1;
    }
    remains_steps *= 2;

    logger.write("Remains steps: %d", remains_steps);

    if (remains_steps != 0) {
        //Set_timer_speed(axis_timer, TIMER_TRACKING_SPEED);
        Set_timer_speed(axis_timer, TIMER_MOVE_SPEED);
        Start();
    }

    return 0;
}

int Axis::GoTo_degree(double degree)
{

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
    if (remains_steps > 0) {
        remains_steps--;
        axis_motor->ToggleStepPin();
    } else {
        current_arcsec = target_arcsec;
        Stop();
    }
    return 0;
}