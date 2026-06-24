#include "avr8_model.hpp"

#include <cmath>
#include <algorithm>
#include <cstdint>
#include <string>

// AVR8 UART: divisor entero puro
// Normal (U2X=0): baud = clock / (16 * (UBRR + 1))  → UBRR = clock/(16*baud) - 1
// Double (U2X=1): baud = clock / ( 8 * (UBRR + 1))  → UBRR = clock/( 8*baud) - 1
// UBRR: 12 bits (0–4095)
// Se calcula ambos modos y se elige el de menor error.

static constexpr uint32_t UBRR_MAX  = 4095;
static constexpr double   MAX_ERROR = 2.0;

static UartResult eval_avr(double clock, double baud, int oversampling, bool u2x)
{
    UartResult r;

    double ideal_ubrr = clock / (static_cast<double>(oversampling) * baud) - 1.0;

    // Reportar el divisor total (UBRR+1) — más útil para el usuario
    r.ideal_div = ideal_ubrr + 1.0;

    // Redondear y clamp explícito a 12 bits (0–4095)
    double ubrr_d = std::round(ideal_ubrr);
    ubrr_d = std::max(0.0, std::min(ubrr_d, static_cast<double>(UBRR_MAX)));
    uint32_t ubrr = static_cast<uint32_t>(ubrr_d);

    r.actual_baud = clock / (static_cast<double>(oversampling) * (ubrr + 1));
    r.error_pct   = std::fabs(100.0 * (r.actual_baud - baud) / baud);
    r.reg_value   = ubrr;
    r.ok          = (r.error_pct <= MAX_ERROR);
    r.mode_note   = u2x ? "U2X=1 (Double Speed, 8x Oversampling)"
                        : "U2X=0 (Normal Speed, 16x Oversampling)";

    return r;
}

UartResult Avr8Model::compute(double clock, double baud)
{
    // Calcular ambos modos
    UartResult normal = eval_avr(clock, baud, 16, false);
    UartResult u2x    = eval_avr(clock, baud,  8, true);
    // Si uno es válido y el otro no, ganar por KO directo
    if (normal.ok && !u2x.ok)  return normal;
    if (!normal.ok && u2x.ok)  return u2x;
    // Si ambos son válidos, o ambos están fuera de rango,
    // elegimos estrictamente el que entregue el menor desvío.
    return (u2x.error_pct < normal.error_pct) ? u2x : normal;
}
