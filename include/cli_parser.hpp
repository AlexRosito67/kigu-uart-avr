#pragma once

#include <string>

static constexpr double STANDARD_BAUDS[] = {
    300, 1200, 2400, 4800, 9600, 14400, 19200, 28800,
    38400, 57600, 115200, 230400, 460800, 921600, 1000000, 2000000, 250000, 500000
};

struct CliArgs {
    double      clock        = 0.0;
    double      baud         = 0.0;
    bool        show_help    = false;
    bool        list_bauds   = false;
    bool        valid        = false;
};

CliArgs parse_args(int argc, char** argv);
double  parse_freq(const std::string& s);
bool    is_standard_baud(double baud);
void    print_help();
void    print_standard_bauds();
