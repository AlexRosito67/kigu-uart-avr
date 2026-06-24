# kigu-uart-avr Manual

**Version 1.0.0**
Rosito Bench — Tiny tools. Big solutions.

---

## Overview

kigu-uart-avr calculates the exact UART baud rate divisor for your AVR8 MCU given a source clock and desired baud rate. It selects the optimal hardware mode automatically (U2X for AVR) and reports the register value ready to copy into your firmware.

---

## How UART baud rate works

UART is asynchronous — both ends agree on a baud rate without sharing a clock. The MCU derives the baud rate by dividing its peripheral clock by a divisor stored in a register:

```
baud_actual = clock / (oversampling × divisor)
```

The divisor must be an integer (or close to one), so there is always a small quantization error. Most UART peripherals tolerate up to 2% error before framing errors occur.

---

## Models

### AVR8

Covers Atmel/Microchip AVR 8-bit microcontrollers (ATmega, ATtiny families).

- **Clock source:** external crystal or internal RC oscillator — user provided
- **Oversampling:** 16× normal (U2X=0) or 8× double speed (U2X=1) — selected automatically
- **Divider:** integer — `UBRR` register (12 bits, 0–4095)
- **Formula:** `UBRR = clock / (oversampling × baud) - 1`

```bash
kigu-uart-avr --clock 16M  --baud 9600
kigu-uart-avr --clock 11059200 --baud 115200
```

> **Note:** 16 MHz crystals produce 3.5% error at 115200 baud — outside the safe range. Use an 11.0592 MHz crystal for exact 115200 baud with AVR.

---

## Interpreting the output

```
  Target baud : 115200.00     ← requested baud rate
  Actual baud : 115273.78     ← achievable baud rate with quantized divisor
  Error       : 0.06 %        ← percentage error (< 2% is acceptable)
  Ideal div   : 43.40         ← exact divisor before quantization
  Register    : 0x000002B6    ← value to write to the hardware register
  Mode        : UART_CLKDIV=43  UART_CLKDIV_FRAG=6
  Status      : OK            ← OK if error ≤ 2%, ERROR otherwise
```

A **Status: ERROR** means the baud error exceeds 2% and framing errors are likely. Try a different clock source or a standard baud rate that divides more cleanly from your clock.

---

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | OK |
| `1` | Argument or usage error |
| `2` | Baud error exceeds 2% |

Exit code `2` allows scripting:

```bash
kigu-uart-avr --clock 16M --baud 115200 || echo "Choose a different clock"
```

---

## Common clock / baud combinations

| MCU | Clock | Baud | Error | Notes |
|-----|-------|------|-------|-------|
| ATmega328P | 11.0592 MHz | 115200 | 0.00% | Exact |
| ATmega328P | 16 MHz | 115200 | 3.55% | Too high |
| ATmega328P | 16 MHz | 9600 | 0.16% | OK |

---

## Support

**hasaki.io@proton.me**

*kigu-uart — Rosito Bench — Tiny tools. Big solutions.*
