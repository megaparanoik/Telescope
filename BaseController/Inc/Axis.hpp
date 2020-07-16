#pragma once

#include "drv8825.hpp"

class Axis
{
private:
    drv8825 *motor;

public:
    Axis(drv8825 *motor);
    ~Axis();
};

