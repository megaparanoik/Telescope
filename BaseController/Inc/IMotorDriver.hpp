#pragma once
#include "stm32f103xb.h"

struct MotorDriverPins
{
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
};

struct ControlPin
{
    GPIO_TypeDef *PORT;
    uint16_t PIN;
};

enum ControlPinNames
{
    RESET_control_pin = 0,
    SLEEP_control_pin,
    ENABLE_control_pin,
    DIR_control_pin,
    STEP_control_pin,
    M0_control_pin,
    M1_control_pin,
    M2_control_pin,
    FAULT_control_pin,
    MAX_control_pin,
};


class IMotorDriver
{
public:
    static const int STEP_1_1   = 1;
    static const int STEP_1_2   = 2;
    static const int STEP_1_4   = 4;
    static const int STEP_1_8   = 8;
    static const int STEP_1_16  = 16;
    static const int STEP_1_32  = 32;

    static const int DIRECTION_CLOCKWISE            = 0;
    static const int DIRECTION_COUNTERCLOCKWISE     = 1;

    static const int SLEEP_MODE_SLEEP   = 0;
    static const int SLEEP_MODE_WAKE    = 1;

    virtual int Reset() = 0;
    virtual int Enable() = 0;
    virtual int Disable() = 0;
    virtual int SetDirection(int direction) = 0;
    virtual int SetSleepMode(int mode) = 0;
    virtual int SetMicrostep(int microstep) = 0;
    virtual int doStep() = 0;
    virtual int doStep(int ms_delay) = 0;
    virtual int ToggleStepPin() = 0;
};

