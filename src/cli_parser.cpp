#include "cli_parser.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <cctype>
#include <algorithm>
#include <cmath>

bool is_standard_baud(double baud)
{
    for (double b : STANDARD_BAUDS)
        if (std::fabs(baud - b) < 0.5) return true;
    return false;
}

void print_standard_bauds()
{
    std::cout << "\n  Standard baud rates:\n  ";
    bool first = true;
    for (double b : STANDARD_BAUDS) {
        if (!first) std::cout << ", ";
        std::cout << static_cast<long>(b);
        first = false;
    }
    std::cout << "\n\n";
}

double parse_freq(const std::string& s)
{
    // Acepta: "80M", "80000000", "16M", "48000000", "40M", "72M"
    std::string upper = s;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    double multiplier = 1.0;
    std::string digits = upper;

    if (!upper.empty() && upper.back() == 'M') {
        multiplier = 1e6;
        digits = upper.substr(0, upper.size() - 1);
    } else if (!upper.empty() && upper.back() == 'K') {
        multiplier = 1e3;
        digits = upper.substr(0, upper.size() - 1);
    }

    return std::stod(digits) * multiplier;
}

void print_help()
{
    std::cout <<
        "\nkigu-uart-avr — UART baud rate calculator for embedded systems\n"
        "Rosito Bench — Tiny tools. Big solutions.\n\n"
        "Usage:\n"
        "  kigu-uart-avr --clock <FREQ> --baud <RATE> [options]\n\n"
        "Models:\n"
        "  AVR8     Atmel/Microchip AVR 8-bit (integer, auto U2X)\n"
        "Required:\n"
        "  --clock  <freq>   Clock frequency (e.g. 80M, 16000000, 72M)\n"
        "  --baud   <rate>   Desired baud rate (e.g. 115200, 9600)\n\n"
        "Examples:\n"
        "  kigu-uart-avr --clock 16M  --baud 9600\n"
        "  kigu-uart-avr --clock 11059200 --baud 115200  # 0.00% error — classic serial crystal\n\n"
        "  -h, --help       Show this help\n"
        "  --list-bauds     List standard baud rates\n\n";
}

CliArgs parse_args(int argc, char** argv)
{
    CliArgs args;

    if (argc < 2) {
        args.show_help = true;
        return args;
    }

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];

        if (a == "-h" || a == "--help") {
            args.show_help = true;
            return args;
        }

        if (a == "--list-bauds") {
            args.list_bauds = true;
            return args;
        }

        auto next = [&]() -> std::string {
            if (i + 1 >= argc)
                throw std::runtime_error("Missing value for " + a);
            return argv[++i];
        };

        if (a == "--clock") {
            args.clock = parse_freq(next());
        } else if (a == "--baud") {
            args.baud = std::stod(next());
        } else {
            throw std::runtime_error("Unknown argument: " + a);
        }
    }

    // Validar argumentos requeridos
    if (args.clock <= 0.0)
        throw std::runtime_error("--clock is required and must be > 0");
    if (args.baud <= 0.0)
        throw std::runtime_error("--baud is required and must be > 0");
    args.valid = true;
    return args;
}
