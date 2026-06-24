#pragma once

#include "model_base.hpp"

class Avr8Model : public ModelBase {
public:
    UartResult compute(double clock, double baud) override;
};
