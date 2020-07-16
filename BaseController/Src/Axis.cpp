#include "Axis.hpp"


Axis::Axis(drv8825 *motor)
{
    this->motor = motor;
}

Axis::~Axis()
{
}