#include "Logger.hpp"
#include "cstdarg"
#include "cstdio"

Logger::Logger()
{
}

int Logger::Init(UART_HandleTypeDef *huart)
{
    uart_device = huart;
    return 0;
}

int Logger::Write(char *message, ...)
{
    char buf[250];
    int len;

    va_list argptr;
    va_start(argptr, message);
    len = vsnprintf(buf, 250, message, argptr);
    buf[len] = '\n';
    buf[len+1] = '\r';

    HAL_UART_Transmit_DMA(uart_device, (uint8_t *)buf, len + 2 );

    return 0;
}
