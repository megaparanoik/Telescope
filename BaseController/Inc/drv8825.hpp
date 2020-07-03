#pragma once

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"

class drv8825
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

public:
    drv8825();
    ~drv8825();

    static const int SLEEP_MODE_SLEEP   = 0;
    static const int SLEEP_MODE_WAKE    = 1;

    static const int STEP_1_1   = 1;
    static const int STEP_1_2   = 2;
    static const int STEP_1_4   = 4;
    static const int STEP_1_8   = 8;
    static const int STEP_1_16  = 16;
    static const int STEP_1_32  = 32;

    static const int DIRECTION_CLOCKWISE            = 0;
    static const int DIRECTION_COUNTERCLOCKWISE     = 1;



    int SetupResetPin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupSleepPin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupEnablePin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupDirPin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupStepPin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupM0Pin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupM1Pin(GPIO_TypeDef *Port, uint16_t Pin);
    int SetupM2Pin(GPIO_TypeDef *Port, uint16_t Pin);

    int SetMicrostep(int microstep);
    int Reset(void);
    int Enable(void);
    int Disable(void);
    int SetSleepMode(int mode);
    int SetDirection(int direction);
    int doStep(void);
};


