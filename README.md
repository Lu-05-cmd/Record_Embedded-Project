# STM32F103 Bare-Metal Drivers

A bare-metal driver library and examples for STM32F103xx (Cortex-M3) microcontrollers. The project accesses hardware registers directly and does not use STM32 HAL, CMSIS, or the standard C runtime. Its goal is to provide essential peripheral drivers, startup code, and a minimal firmware build/flash workflow.

## Current demo

The firmware is built from `test/test.c`. The program:

- configures HSE and a x9 PLL as the system clock;
- enables the GPIOA, AFIO, and TIM1 clocks;
- configures PA9 as an alternate-function push-pull output;
- outputs TIM1 channel 2 PWM on PA9 with prescaler `71` and auto-reload `19999`;
- alternates compare values `1000` and `2000` to change pulse width, with a software delay between states.

`application/core/main.c` is a standalone PC13 LED example and is not currently included in the `makefile` source list.

## Components

| Area | Contents |
| --- | --- |
| `application/core/startup` | Vector table, `Reset_Handler`, `.data`/`.bss` initialization, and default handlers. |
| `application/core/linker` | Linker script for medium-density STM32F103 devices: 64 KiB Flash and 20 KiB RAM. |
| `application/core/nvic`, `application/core/scb` | NVIC and SCB configuration. |
| `application/driver/stm32f103` | STM32F103xx register definitions and GPIO, RCC, timer/PWM, AFIO, USART, ADC, SPI, I2C, EXTI, and DMA drivers. |
| `application/config/macros` | Bit-manipulation macros. |
| `test/test.c` | PWM demo firmware entry point. |
| `docs` | Datasheets, reference manuals, and Flash/Cortex-M3 documentation. |

## Requirements

- `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain)
- `make`
- OpenOCD for programming and debugging through ST-Link
- An STM32F103xx board compatible with the linker script's 64 KiB Flash / 20 KiB RAM layout

Ensure that `arm-none-eabi-gcc`, `arm-none-eabi-gdb`, `make`, and `openocd` are available in `PATH`.

## Build

Build the firmware without programming the board:

```sh
make build/firmware.elf
```

This creates `build/firmware.elf`. The default `make` target (or `make all`) builds the firmware and then runs OpenOCD to program it through ST-Link:

```sh
make
```

## Program and debug

Program an already built firmware image:

```sh
make flash
```

Start OpenOCD and wait for a GDB connection:

```sh
make debug-server
```

Open GNU GDB with the ELF file:

```sh
make debug
```

In another terminal, connect GDB to OpenOCD (default port: 3333):

```gdb
target extended-remote :3333
monitor reset halt
load
continue
```

## Directory layout

```text
.
|-- application
|   |-- config/macros/bit_ops.h
|   |-- core
|   |   |-- linker/link.ld
|   |   |-- nvic/
|   |   |-- scb/
|   |   `-- startup/startup_stm32f103.asm
|   `-- driver/stm32f103
|       |-- Inc/
|       |-- Src/
|       `-- stm32f103xx.h
|-- docs
|-- test/test.c
|-- makefile
`-- README.md
```

## Notes

- The demo's pin, clock, and peripheral configuration targets PA9/TIM1 channel 2. Check the board schematic before programming.
- Not every API declared in the headers is used by the demo. Review the driver source and reference manual before using an API on hardware.
- `make clean` uses `rm`; on Windows, use an environment that provides it (for example Git Bash, MSYS2, or WSL), or adjust the makefile.
