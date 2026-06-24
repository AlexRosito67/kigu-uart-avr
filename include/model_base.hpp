#pragma once

#include "uart_result.hpp"

class ModelBase {
public:
    virtual UartResult compute(double clock, double baud) = 0;
    virtual ~ModelBase() = default;
};
