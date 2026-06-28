# kigu-uart-avr 器具

<p align="center">
  <img src="./logo_kigu.png" width="600" alt="Kigu Logo">
</p>
<p align="center">
  <strong>Tiny tools. Big solutions.</strong>
</p>



**UART baud rate calculator for embedded systems developers.**

No browser. No online tool. No guessing. Run it from your terminal, get the exact register value for your AVR8 MCU.

Part of the **Kigu** toolchain by [Rosito Bench](https://github.com/AlexRosito67).

>**kigu-uart-avr is free — AVR8 only.**
 **To download your free binaries please go to:** [Rosito Bench Store](https://hasaki.lemonsqueezy.com).**

---

## Supported families

| Model | MCU Family | Divider | Auto mode |
|-------|-----------|---------|-----------|
| `AVR8` | Atmel/Microchip AVR 8-bit | Integer | U2X auto |

---

## Usage

```bash
kigu-uart-avr --clock <FREQ> --baud <RATE> [options]
```

### Examples

```bash

# ATmega328P at 11.0592 MHz (exact 115200 baud)
kigu-uart-avr --clock 11059200 --baud 115200

```

### Sample output

```
  kigu-uart v1.0.0 — Rosito Bench
  -----------------------------------------------------
  Target baud : 115200.00
  Actual baud : 115200.00
  Error       : 0.00 %
  Ideal div   : 6.00
  Register    : 0x00000005  (5)
  Mode        : U2X=0 (Normal Speed, 16x Oversampling)
  Status      : OK
  -----------------------------------------------------
```

---

## Options

| Flag | Description |
|------|-------------|
| `--clock` | Source clock frequency — e.g. `80M`, `16M`, `11059200` (required) |
| `--baud` | Desired baud rate (required) |
| `--list-bauds` | List standard baud rates (optional) |
| `-h`, `--help` | Show help |

---

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | OK — baud error within 2% |
| `1` | Argument error |
| `2` | Baud error exceeds 2% |

---

## License

kigu-uart-avr is free — AVR8 only.
For ESP32, STM32, and GENERIC support, see [kigu-uart Pro](https://hasaki.lemonsqueezy.com).

*Rosito Bench — Tiny tools. Big solutions.*
