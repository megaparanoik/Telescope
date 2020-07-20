#pragma once

#include "main.h"

class Logger
{
private:
    Logger();
    Logger(const Logger&);
    Logger& operator=(Logger&);

    UART_HandleTypeDef *uart_device;

public:
    static Logger& GetInstance() {
        static Logger instance;
        return instance;
    }

    int Init(UART_HandleTypeDef *huart);
    int Write(char *message, ...);    
};

