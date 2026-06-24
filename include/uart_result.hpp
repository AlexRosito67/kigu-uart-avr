#pragma once

#include <cstdint>
#include <string>

struct UartResult {
    double   ideal_div   = 0.0;
    double   actual_baud = 0.0;
    double   error_pct   = 0.0;
    uint32_t reg_value   = 0;       // valor listo para escribir al registro
    bool     ok          = false;
    std::string mode_note;          // "U2X enabled", "OVER8=1", etc.
};
