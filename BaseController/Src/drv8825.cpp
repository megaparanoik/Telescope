#include <drv8825.hpp>


drv8825::drv8825()
{
}

drv8825::drv8825(struct MotorDriverPins *pins)
{
    Reset_PORT  = pins->Reset_PORT;
    Reset_PIN   = pins->Reset_PIN;
    Sleep_PORT  = pins->Sleep_PORT;
    Sleep_PIN   = pins->Sleep_PIN;
    Enable_PORT = pins->Enable_PORT;
    Enable_PIN  = pins->Enable_PIN;
    Dir_PORT    = pins->Dir_PORT;
    Dir_PIN     = pins->Dir_PIN;
    Step_PORT   = pins->Step_PORT;
    Step_PIN    = pins->Step_PIN;
    M0_PORT     = pins->M0_PORT;
    M0_PIN      = pins->M0_PIN;
    M1_PORT     = pins->M1_PORT;
    M1_PIN      = pins->M1_PIN;
    M2_PORT     = pins->M2_PORT;
    M2_PIN      = pins->M2_PIN; 
}

drv8825::~drv8825()
{
}

int drv8825::SetMicrostep(int microstep)
{
    switch (microstep)
    {
    case STEP_1_1:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_RESET);  //M0 - 0
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_RESET);  //M1 - 0
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_RESET);  //M2 - 0
        break;

    case STEP_1_2:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_SET);    //M0 - 1
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_RESET);  //M1 - 0
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_RESET);  //M2 - 0
        break;
  
    case STEP_1_4:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_RESET);  //M0 - 0
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_SET);    //M1 - 1
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_RESET);  //M2 - 0
        break;
  
    case STEP_1_8:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_SET);    //M0 - 1
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_SET);    //M1 - 1
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_RESET);  //M2 - 0
        break;
  
    case STEP_1_16:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_RESET);  //M0 - 0
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_RESET);  //M1 - 0
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_SET);    //M2 - 1
  
    case STEP_1_32:
        HAL_GPIO_WritePin(M0_PORT, M0_PIN, GPIO_PIN_SET);    //M0 - 1
        HAL_GPIO_WritePin(M1_PORT, M1_PIN, GPIO_PIN_RESET);  //M1 - 0
        HAL_GPIO_WritePin(M2_PORT, M2_PIN, GPIO_PIN_SET);    //M2 - 1
        break;

    default:
        break;
    }

    return 0;
}

int drv8825::Reset(void)
{
    HAL_GPIO_WritePin(Reset_PORT, Reset_PIN, GPIO_PIN_RESET );   //Reset - 0
    HAL_GPIO_WritePin(Reset_PORT, Reset_PIN, GPIO_PIN_SET);      //Reset - 1 

    return 0;
}

int drv8825::Enable(void)
{
    HAL_GPIO_WritePin(Enable_PORT, Enable_PIN, GPIO_PIN_RESET); //Enable - 0
    return 0;
}

int drv8825::Disable(void)
{
    HAL_GPIO_WritePin(Enable_PORT, Enable_PIN, GPIO_PIN_SET); //Enable - 1
    return 0;
}

int drv8825::SetSleepMode(int mode)
{
    switch (mode)
    {
    case SLEEP_MODE_SLEEP:
        HAL_GPIO_WritePin(Sleep_PORT, Sleep_PIN, GPIO_PIN_RESET);
        break;
    
    case SLEEP_MODE_WAKE:
        HAL_GPIO_WritePin(Sleep_PORT, Sleep_PIN, GPIO_PIN_SET);
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
        HAL_GPIO_WritePin(Dir_PORT, Dir_PIN, GPIO_PIN_RESET);
        break;

    case DIRECTION_COUNTERCLOCKWISE:
        HAL_GPIO_WritePin(Dir_PORT, Dir_PIN, GPIO_PIN_SET);
        break;
    
    default:
        break;
    }

    return 0;
}

int drv8825::doStep()
{
    HAL_GPIO_WritePin(Step_PORT, Step_PIN, GPIO_PIN_SET);   //Step - 1
    //10.06us @ 72MHz
    for(int i = 0; i < 78; ) {
        i++;
    }
    HAL_GPIO_WritePin(Step_PORT, Step_PIN, GPIO_PIN_RESET); //Step - 0 
    return 0;
}

int drv8825::doStep(int ms_delay)
{
    HAL_GPIO_WritePin(Step_PORT, Step_PIN, GPIO_PIN_SET);   //Step - 1
    LL_mDelay(ms_delay);
    HAL_GPIO_WritePin(Step_PORT, Step_PIN, GPIO_PIN_RESET); //Step - 0 
    return 0;
}

int drv8825::ToggleStepPin()
{
    HAL_GPIO_TogglePin(Step_PORT, Step_PIN);
    return 0;
}

double drv8825::GetResolution()
{
    return resolution;
}