#include "cli_parser.hpp"
#include "uart_result.hpp"
#include "avr8_model.hpp"
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include "model_base.hpp"

#ifndef KIGU_UART_AVR_VERSION
#define KIGU_UART_AVR_VERSION "dev"
#endif

static void print_result(const UartResult& r, double target_baud)
{
    std::cout << "\n";
    std::cout << "  kigu-uart-avr v" KIGU_UART_AVR_VERSION " — Rosito Bench\n";
    std::cout << "  -----------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  Target baud : " << target_baud        << "\n";
    std::cout << "  Actual baud : " << r.actual_baud      << "\n";
    std::cout << "  Error       : " << r.error_pct        << " %\n";
    std::cout << "  Ideal div   : " << r.ideal_div        << "\n";
    std::cout << "  Register    : 0x" << std::hex << std::uppercase
              << std::setw(8) << std::setfill('0') << r.reg_value
              << std::dec << "  (" << r.reg_value << ")\n";
    std::cout << "  Mode        : " << r.mode_note        << "\n";
    std::cout << "  Status      : " << (r.ok ? "OK" : "ERROR — baud error exceeds 2%") << "\n";
    std::cout << "  -----------------------------------------------------\n\n";
}

int main(int argc, char** argv)
{
    
    CliArgs args;

    try {
        args = parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "[kigu-uart-avr] " << e.what() << "\n";
        print_help();
        return 1;
    }

    if (args.show_help) {
        print_help();
        return 0;
    }

    if (args.list_bauds) {
        print_standard_bauds();
        return 0;
    }

    if (!args.valid)
        return 1;

    // Warning si el baud no es estándar
    if (!is_standard_baud(args.baud))
        std::cerr << "  [WARNING] " << static_cast<long>(args.baud)
                  << " is not a standard baud rate. Use --list-bauds to see standard values.\n";

    // Crear modelo
    std::unique_ptr<ModelBase> model;

    model = std::make_unique<Avr8Model>();    

    UartResult result = model->compute(args.clock, args.baud);
    print_result(result, args.baud);

    return result.ok ? 0 : 2;
}
