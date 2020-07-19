#pragma once

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_utils.h"
#include "IMotorDriver.hpp"

class drv8825 : public IMotorDriver
{
private:
    GPIO_TypeDef *Reset_PORT;
    uint16_t Reset_PIN;

    GPIO_TypeDef *Sleep_PORT;
    uint16_t Sleep_PIN;

    GPIO_TypeDef *Enable_PORT;
    uint16_t Enable_PIN;

    GPIO_TypeDef *Dir_PORT;
    uint16_t Dir_PIN;

    GPIO_TypeDef *Step_PORT;
    uint16_t Step_PIN;

    GPIO_TypeDef *M0_PORT;
    uint16_t M0_PIN;
    GPIO_TypeDef *M1_PORT;
    uint16_t M1_PIN;
    GPIO_TypeDef *M2_PORT;
    uint16_t M2_PIN;

    GPIO_TypeDef *FAULT_PORT;
    uint16_t FAULT_PIN;

    double resolution = 0.7788461538;

public:
    drv8825();
    drv8825(struct MotorDriverPins *pins);
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


