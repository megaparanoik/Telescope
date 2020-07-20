#pragma once

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "IMotorDriver.hpp"

class drv8825 : public IMotorDriver
{
private:
    struct ControlPin Motor_pins[MAX_control_pin];
    double resolution = 0.7788461538;
    int InitPortOutput(GPIO_TypeDef *port, uint16_t pins);

public:
    drv8825();
    drv8825(struct ControlPin pins[]);
    ~drv8825();

    int Reset(void);
    int Enable(void);
    int Disable(void);
    int SetSleepMode(int mode);
    int SetDirection(int direction);
    int SetMicrostep(int microstep);
    int doStep();
    int doStep(int ms_delay);
    int ToggleStepPin();
    double GetResolution();
};


