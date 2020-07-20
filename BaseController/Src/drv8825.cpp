#include <drv8825.hpp>
#include <string.h>

drv8825::drv8825()
{
}

drv8825::drv8825(struct ControlPin pins[])
{  
    for (int i = RESET_control_pin; i < MAX_control_pin; i++) {
        Motor_pins[i] = pins[i];
    }

    uint16_t pins_A = 0;
    uint16_t pins_B = 0;
    uint16_t pins_C = 0;
    uint16_t pins_D = 0;

    //TODO: FAULT_control_pin - special case : input
    for (int i = RESET_control_pin; i < FAULT_control_pin; i++) {
        if (pins[i].PORT == GPIOA) {
            pins_A |= pins[i].PIN;
        } else if (pins[i].PORT == GPIOB) {
            pins_B |= pins[i].PIN;
        } else if (pins[i].PORT == GPIOC) {
            pins_C |= pins[i].PIN;
        } else if (pins[i].PORT == GPIOD) {
            pins_D |= pins[i].PIN;
        }
    }

    if (pins_A != 0) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        InitPortOutput(GPIOA, pins_A);
    }
    if (pins_B != 0) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        InitPortOutput(GPIOB, pins_B);
    }
    if (pins_C != 0) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        InitPortOutput(GPIOC, pins_C);
    }
    if (pins_D != 0) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        InitPortOutput(GPIOD, pins_D);
    }

}

drv8825::~drv8825()
{
}

int drv8825::InitPortOutput(GPIO_TypeDef *port, uint16_t pins)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(port, pins, GPIO_PIN_RESET);
        
    GPIO_InitStruct.Pin = pins;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    return 0;
}

int drv8825::SetMicrostep(int microstep)
{
    switch (microstep)
    {
    case STEP_1_1:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_RESET);
        break;

    case STEP_1_2:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_RESET);
        break;
  
    case STEP_1_4:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_RESET);
        break;
  
    case STEP_1_8:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_RESET);
        break;
  
    case STEP_1_16:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_SET);
        break;
  
    case STEP_1_32:
        HAL_GPIO_WritePin(Motor_pins[M0_control_pin].PORT, Motor_pins[M0_control_pin].PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Motor_pins[M1_control_pin].PORT, Motor_pins[M1_control_pin].PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Motor_pins[M2_control_pin].PORT, Motor_pins[M2_control_pin].PIN, GPIO_PIN_SET);
        break;

    default:
        break;
    }

    return 0;
}

int drv8825::Reset(void)
{
    HAL_GPIO_WritePin(Motor_pins[RESET_control_pin].PORT, Motor_pins[RESET_control_pin].PIN, GPIO_PIN_RESET);   //Reset - 0
    HAL_GPIO_WritePin(Motor_pins[RESET_control_pin].PORT, Motor_pins[RESET_control_pin].PIN, GPIO_PIN_SET);      //Reset - 1 

    return 0;
}

int drv8825::Enable(void)
{
    HAL_GPIO_WritePin(Motor_pins[ENABLE_control_pin].PORT, Motor_pins[ENABLE_control_pin].PIN, GPIO_PIN_RESET);
    return 0;
}

int drv8825::Disable(void)
{
    HAL_GPIO_WritePin(Motor_pins[ENABLE_control_pin].PORT, Motor_pins[ENABLE_control_pin].PIN, GPIO_PIN_SET);
    return 0;
}

int drv8825::SetSleepMode(int mode)
{
    switch (mode)
    {
    case SLEEP_MODE_SLEEP:
        HAL_GPIO_WritePin(Motor_pins[SLEEP_control_pin].PORT, Motor_pins[SLEEP_control_pin].PIN, GPIO_PIN_RESET);
        break;
    
    case SLEEP_MODE_WAKE:
        HAL_GPIO_WritePin(Motor_pins[SLEEP_control_pin].PORT, Motor_pins[SLEEP_control_pin].PIN, GPIO_PIN_SET);
        break;

    default:
        break;
    }

    return 0;
}

int drv8825::SetDirection(int direction)
{
    switch (direction)
    {
    case DIRECTION_CLOCKWISE:
        HAL_GPIO_WritePin(Motor_pins[DIR_control_pin].PORT, Motor_pins[DIR_control_pin].PIN, GPIO_PIN_RESET);
        break;

    case DIRECTION_COUNTERCLOCKWISE:
        HAL_GPIO_WritePin(Motor_pins[DIR_control_pin].PORT, Motor_pins[DIR_control_pin].PIN, GPIO_PIN_SET);
        break;
    
    default:
        break;
    }

    return 0;
}

int drv8825::doStep()
{
    HAL_GPIO_WritePin(Motor_pins[STEP_control_pin].PORT, Motor_pins[STEP_control_pin].PIN, GPIO_PIN_SET);
    //10.06us @ 72MHz
    for(int i = 0; i < 78; ) {
        i++;
    }
    HAL_GPIO_WritePin(Motor_pins[STEP_control_pin].PORT, Motor_pins[STEP_control_pin].PIN, GPIO_PIN_RESET);
    return 0;
}

int drv8825::doStep(int ms_delay)
{
    HAL_GPIO_WritePin(Motor_pins[STEP_control_pin].PORT, Motor_pins[STEP_control_pin].PIN, GPIO_PIN_SET);
    LL_mDelay(ms_delay);
    HAL_GPIO_WritePin(Motor_pins[STEP_control_pin].PORT, Motor_pins[STEP_control_pin].PIN, GPIO_PIN_RESET);
    return 0;
}

int drv8825::ToggleStepPin()
{
    HAL_GPIO_TogglePin(Motor_pins[STEP_control_pin].PORT, Motor_pins[STEP_control_pin].PIN);
    return 0;
}

double drv8825::GetResolution()
{
    return resolution;
}